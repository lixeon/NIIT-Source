/*
 *	Util.java	1.7	99/04/06 SMI
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

//import java.io.PrintStream;
// Miscellaneous support routines

import vm.Const;

public
class Util
{
    // How many bytes of storage for a particular signature?
    static public int argsSize(String sig) {
	int argsSize = 0;

	for (int pos = 0; sig.charAt(pos) != Const.SIGC_ENDMETHOD; pos++) {
	    switch (sig.charAt(pos)) {
	      case Const.SIGC_BOOLEAN:
	      case Const.SIGC_BYTE:
	      case Const.SIGC_CHAR:
	      case Const.SIGC_SHORT:
	      case Const.SIGC_INT:
	      case Const.SIGC_FLOAT:
		argsSize++;
		break;

	      case Const.SIGC_LONG:
	      case Const.SIGC_DOUBLE:
		argsSize += 2;
		break;

	      case Const.SIGC_CLASS:
		argsSize++;
		while (sig.charAt(pos) != Const.SIGC_ENDCLASS) {
		    pos++;
		}
		break;

	      case Const.SIGC_ARRAY:
		argsSize++;
		while (sig.charAt(pos) == Const.SIGC_ARRAY) {
		    pos++;
		}

		if (sig.charAt(pos) == Const.SIGC_CLASS) {
		    while (sig.charAt(pos) != Const.SIGC_ENDCLASS) {
			pos++;
		    }
		}
		break;

	      case Const.SIGC_METHOD:
		break;

	      default:
		System.err.println("Error: unparseable signature: " + sig);
		System.exit(3);
	    }

	}

	return argsSize;
    }
    

    // Replace characters C doesn't like with underscores
    public static String convertToClassName(String name) {
	char chars[] = name.toCharArray();

	for (int i = 0; i < chars.length; i++) {
	    switch (chars[i]){
	    case '/':
	    case '.':
	    case '$':
		chars[i] = '_';
	    }
	}

	return String.valueOf(chars);
    }


    public static String accessToString( int access ){
	String result = "";
        if ((access & Const.ACC_PUBLIC) != 0){
	    result += "public ";
	}
	if ((access & Const.ACC_PRIVATE) != 0){
	    result += "private ";
	}
	if ((access & Const.ACC_PROTECTED) != 0){
	    result += "protected ";
	}
	if ((access & Const.ACC_STATIC) != 0){
	    result += "static ";
	}
	if ((access & Const.ACC_TRANSIENT) != 0){
	    result += "transient ";
	}
	if ((access & Const.ACC_SYNCHRONIZED) != 0){
	    result += "synchronized ";
	}
	if ((access & Const.ACC_ABSTRACT) != 0){
	    result += "abstract ";
	}
	if ((access & Const.ACC_NATIVE) != 0){
	    result += "native ";
	}
	if ((access & Const.ACC_FINAL) != 0){
	    result += "final ";
	}
	if ((access & Const.ACC_VOLATILE) != 0){
	    result += "volatile ";
	}
	if ((access & Const.ACC_INTERFACE) != 0){
	    result += "interface ";
	}
	return result;
    }


    //
    // given a full-Unicode Java String value, produce a byte array
    // containing the UTF-8 encoding of it.
    //
    public static String unicodeToUTF( String in ){
	int n = in.length();
	StringBuffer t = new StringBuffer( 3*n ); // conservative sizing.
	int nt = 0;
	for( int i = 0; i < n; i++ ){
	    char c = in.charAt(i);
	    if ( c == '\u0000' ){
		t.append('\u00c0');
		t.append('\u0080');
		nt+=2;
	    } else if ( c <= '\u007f' ){
		t.append(c);
		nt+=1;
	    } else if ( c <= '\u07ff' ){
		t.append((char)('\u00c0' | ( c>>6 )));
		t.append((char)('\u0080' | ( c&0x3f)));
		nt+=2;
	    } else {
		t.append((char)('\u00e0' | ( c>>12 )) );
		t.append((char)('\u0080' | ( (c>>6)&0x3f)) );
		t.append((char)('\u0080' | ( c&0x3f)) );
		nt+=3;
	    }
	}
	if ( nt == n ) return in;
	else return t.toString();
    }

    // Create a JNI external name from the given classname and methodname.
    // If typename is not null, then we are producing a JNI name for an 
    // overloaded method, and we must include the type name, also.
    public static String
    convertToJNIName(String classname, String methodname, String typename) { 
	StringBuffer result = new StringBuffer("Java_");
	stringToJNI(result, classname);
	result.append('_');
	stringToJNI(result, methodname);
	if (typename != null) {
	    result.append("__");
	    // Only include the stuff inside the parentheses.
	    stringToJNI(result, typename.substring(1, typename.indexOf(')')));
	} 
	return result.toString();
    }

    private static void 
    stringToJNI(StringBuffer result, String name) { 
	int length = name.length();
	for (int i = 0; i < length; i++) { 
	    char ch = name.charAt(i);
	    if (ch <= 0x7f && Character.isLetterOrDigit(ch)) {
		result.append(ch);
	    } else { 
		result.append('_');
		switch(ch) { 
		   case '/':  break; // the _ is all we need
		   case '_':  result.append('1'); break;
		   case ';':  result.append('2'); break;
		   case '[':  result.append('3'); break;
		   default: { 
		       // Adding 0x100000 to a 16-bit number forces 
		       // toHexString to produce a string of the form "10xxxx".
		       // Discard the initial "1" to get the right result.
		       String t = Integer.toHexString(ch + 0x100000);
		       result.append(t.substring(1));
		   }
		}
	    }
	}
    }
}
