/*
 * Copyright 1995-2002 by Sun Microsystems, Inc.,
 * 901 San Antonio Road, Palo Alto, California, 94303, U.S.A.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Sun Microsystems, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Sun.
 * Use is subject to license terms.
 */

package runtime;

import java.util.BitSet;
import java.util.Collections;
import java.util.Comparator;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
import java.io.*;


class KVMNameTable extends KVMHashtable { 

    static final String NameStringPrefix = "UString_Item";
    static final int TABLE_SIZE = 256;



    KVMNameTable() { 
        super(TABLE_SIZE, String.class); 
    }

    long hash(Object x) { 
        String str = (String)x;
        return stringHash(str);
    }


    KVMClassTable classTable;
    Hashtable nameExists;
    Hashtable newNames;

    /**
     * If set to true, all entries that may be renamed will be
     * changed to short names such as 'A', or 'Az'
     */
    boolean compactEntries = false;

    void setCompactEntries(boolean value) {
        compactEntries = value;
    }

    void writeTable(CCodeWriter out, String tableName) {
        nameExists = new Hashtable();
        newNames = new Hashtable();
	Vector all = new Vector();
	BitSet seenSizes   = new BitSet();

        // (1) get all known names
	for (int i = 0; i < TABLE_SIZE; i++) { 
	    for (  String entry = (String)getFirst(i); 
		   entry != null; 
		   entry = (String)getNext(entry)) { 
                nameExists.put(entry, entry);
	    }
	}
        // (2) change to shorter names if possible
	for (int i = 0; i < TABLE_SIZE; i++) { 
	    for (  String entry = (String)getFirst(i); 
		   entry != null; 
		   entry = (String)getNext(entry)) { 
                if (compactEntries && mayBeRenamed(entry) && entry.length() > 2) {
                    newNames.put(entry, getShortName(entry));
                } else {
                    newNames.put(entry, entry);
                }
	    }
	}

	for (int i = 0; i < TABLE_SIZE; i++) { 
	    for (  String entry = (String)getFirst(i); 
		   entry != null; 
		   entry = (String)getNext(entry)) { 
		String keyFilled = 
		    Integer.toHexString(getNameKey(entry, true)+0x10000).substring(1);
		all.add(new String[]{entry, keyFilled});
                String newName = (String)newNames.get(entry);
		seenSizes.set(newName.length());
	    }
	}
	for (int i = 0; i < seenSizes.size(); i++) { 
	    if (seenSizes.get(i)) { 
                out.println("DECLARE_USTRING_STRUCT(" + i + ");");
	    }
	}
	Collections.sort(all, new Comparator() { 
	      public int compare(Object o1, Object o2) { 
		  String[] pair1 = (String[])o1;
		  String[] pair2 = (String[])o2;
		  int key1 = getNameKey(pair1[0], true);
		  int key2 = getNameKey(pair2[0], true);
		  return key1 - key2;
	      }
	    });
	out.println();
	out.println();
	out.println("static CONST struct AllUTFStrings_Struct {");
	for (Enumeration e = all.elements(); e.hasMoreElements(); ) { 
	    String[] pair = (String[])e.nextElement(); 
	    String entry = pair[0];
	    String key = pair[1];
            String newName = (String)newNames.get(entry);
            out.println("\tstruct UTF_Hash_Entry_" + newName.length() + 
			" U" + key + ";");
	}
	out.println("} AllUTFStrings = {");
        int totalRn = 0;
	for (Enumeration e = all.elements(); e.hasMoreElements(); ) { 
	    String[] pair = (String[])e.nextElement(); 
	    String entry = pair[0];
	    String key = pair[1];
	    String nextEntryName = getUString((String)getNext(entry));
            String newName = (String)newNames.get(entry);
            out.print("\tUSTRING(" + key + ", " + nextEntryName + ", " + 
		      newName.length() + ", ");
            if (!newName.equals(entry)) {
                out.printSafeString(newName);
                out.print(" /* was " +  entry + " */");
                totalRn += entry.length() - newName.length();
            } else {
                out.printSafeString(entry);
            }
            out.print("),");
	    try { 
		if (entry.charAt(0) < 20) { 
		    String s = classTable.decodeMethodSignature(entry);
		    out.print(" /* ");
		    out.print(s);
		    out.print(" */");
		} 
	    } catch (RuntimeException ex) { }
	    out.println();
	}
	out.println("};\n");
	super.writeTable(out, tableName);
        System.out.println("Total rename saving = " + totalRn);
    }

    int shortNameSz  = 1 + 26 + 26; /* _ A-Z, a-z, 0-9 */
    int shortNameMax = shortNameSz * shortNameSz * shortNameSz;
    int shortNameIdx = 1;

    String getShortName(String oldName) {
        while (shortNameIdx < shortNameMax) {
            int d0, d1, d2;
            d0 = (shortNameIdx /(shortNameSz * shortNameSz)) % shortNameSz;
            d1 = (shortNameIdx / shortNameSz               ) % shortNameSz;
            d2 = (shortNameIdx                             ) % shortNameSz;

            shortNameIdx ++;

            d0 = getShortNameChar(d0);
            d1 = getShortNameChar(d1);
            d2 = getShortNameChar(d2);

            String newName = "";
            if (d0 == '_' && d1 == '_') {
                newName += (char)d2;
            }
            else if (d0 == '_') {
                newName += (char)d1;
                newName += (char)d2;
            }
            else {
                newName += (char)d0;
                newName += (char)d1;
                newName += (char)d2;
            }

            if (nameExists.get(newName) == null) {
                nameExists.put(newName, newName);
                return newName;
            }            
        }

        return oldName;
    }

    int getShortNameChar(int i) {
        if (i == 0) {
            return '_';
        }

        i -= 1;

        if (i < 26) {
            return 'A' + i;
        } else {
            return 'a' + i - 26;
        }
    }

    Object tableChain(CCodeWriter out, int bucket, Object[] list) {
        return list.length == 0 ? null : list[0];
    }

    void tableEntry(CCodeWriter out, int bucket, Object token ) { 
        out.print(getUString((String)token));
    }

    public void addNewEntryCallback(int bucket, Object neww) {
        int value;
        String next = (String)getNext((String)neww);
        if (next == null) { 
            value = bucket + getSize();
        } else { 
            value = getKey(next, true) + getSize();
        }
        setKey(neww, value);
    }

    public String getUString(int key) { 
        String thisKey = Integer.toHexString(key + 0x10000).substring(1);
	return "&AllUTFStrings.U" + thisKey;
    }

    public String getUString(String string) { 
	if (string == null) return "NULL";
        return getUString(getKey(string));
    }

    public int getNameKey(String string, boolean mayBeRenamed) { 
        //System.out.println("getNameKey: " + string);
        return getKey(string, mayBeRenamed);
    }


    public void declareUString(CCodeWriter out, String name) { }

}


