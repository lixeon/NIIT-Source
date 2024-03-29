/*
 *	Str2ID.java	1.10	02/09/27 SMI
 *
 * Copyright (c) 1997 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 * Use is subject to license terms.
 */

package jcc;

import components.UnicodeConstant;
import components.StringConstant;
import util.Localizer;

/*
 * This class duplicates the String to ID hashing that Java
 * does internally.  An initial set of hash tables is produced
 * that the Java runtime will use.
 * (perhaps this really belongs in package vm?)
 */

public
class Str2ID
{
    // The contents of each hash table chain
    public int hashTableSize;
    public int 		   used = 0;
    public int		   baseId = 1;
    public Str2ID	   next = null;
    public UnicodeConstant strings[];
    public StringConstant  parameters[] = null;
    public int 		   hashbits[];
    
    boolean locked = false;	// check that nothing new is added.

    public static final int DEFAULT_hashTableSize = 2003;

    public Str2ID() { 
        this(DEFAULT_hashTableSize, true);
    }

    public Str2ID(int size) { 
        this(size, false);
    }

    private Str2ID(int size, boolean knownPrime) { 
        if (!knownPrime) { 
	    size |= 1;
	    while (!isPrime(size))
	        size += 2;
	}
	hashTableSize = size;
	strings = new UnicodeConstant[size];
	hashbits = new int[size];
	overflowPoint = (int)(hashTableSize * 0.8);
    }

    // Use values in java/util/util.c

    int overflowPoint;
    public static Str2ID sigHash = new Str2ID();


    // Treat val as an unsigned quantity and do the MOD operation
    int unsignedMod(int val) {
	if (val >= 0) {
	    return val % hashTableSize;
	} else {
	    return (int)(((long)val&0xffffffffL) % (long)hashTableSize);
	}
    }
    

    static int hashString(String str) {
	int result = 0;
	for (int i = 0; i < str.length(); i++) {
	    result = result * 37 + (int) str.charAt(i);
	}

	return result;
    }
    

    public int getID( UnicodeConstant str, StringConstant parameter ) {
	Str2ID head = this;
	int index;
	int hash = hashString(str.string);
	int phash = hash & 0x7fffffff;
	int secondaryHash = (hash & 7) + 1;

	Str2ID chain = head;
	for ( ; ; ) {
	    index = chain.unsignedMod(hash);
	    while (chain.strings[index] != null) {
		if ((chain.hashbits[index] == phash) &&
		    chain.strings[index].string.equals(str.string)) {
		    // We found the string, calculate the ID
		    return chain.baseId + index;
		}
		index -= secondaryHash;
		if (index < 0) {
		    index += chain.hashTableSize;
		}
	    }

	    // Didn't find the string in this chain. . . .
	    if (chain.next == null) { 
	        break;
	    } else { 
	        chain = chain.next;
	    }
	}

	if (locked) { 
	    throw new NullPointerException(
		Localizer.getString("str2id.attempting_to_add_to_table_when_locked", str));

	}
	if (chain.used >= chain.overflowPoint) {
	    chain.next = new Str2ID();
	    chain.next.baseId = chain.baseId + chain.hashTableSize;
	    chain = chain.next;
	    index = chain.unsignedMod(hash);
	}

	chain.strings[index] = str;
	chain.hashbits[index] = phash;
	if (parameter != null ){
	    if ( chain.parameters == null ){
		chain.parameters = new StringConstant[chain.hashTableSize];
	    }
	    chain.parameters[index] = parameter;
	}
	chain.used++;
	return chain.baseId + index;
    }



    public int getID(String str) {
	return getID( new UnicodeConstant( str ), (StringConstant)null );
    }


    public int getID(UnicodeConstant name, UnicodeConstant sig) {
	return (getID( name, (StringConstant)null) << 16) + getID( sig, (StringConstant)null);
    }



    public int getID(String name, String sig) {
	return (getID(name) << 16) + getID(sig);
    }

    public void setLocked(boolean lock) { 
        this.locked = lock;
    }

    public Str2ID rehash() {
        Str2ID newchain, chain;
	int count = 0;
	for (chain = this; chain != null; chain = chain.next) {
	    count += chain.used;
	}
	newchain = new Str2ID((int)Math.ceil(1.25 * count));

	for (chain = this; chain != null; chain = chain.next) {
	    UnicodeConstant strings[] = chain.strings;
	    StringConstant  parameters[] = chain.parameters;
	    for (int i = chain.hashTableSize; --i >= 0; ) { 
	        UnicodeConstant string = strings[i];
		if (string != null) 
		    newchain.getID(string, parameters == null ? null : parameters[i]);
	    }
	}
	return newchain;
    }

    static boolean isPrime(int n){
        if ((n % 2) == 0)
	    return false;
	int sqrt = (int) Math.sqrt(n);
	for (int i = 3; i <= sqrt; i += 2) { 
	    if ((n % i) == 0)
	        return false;
	}
	return true;
    }
}
