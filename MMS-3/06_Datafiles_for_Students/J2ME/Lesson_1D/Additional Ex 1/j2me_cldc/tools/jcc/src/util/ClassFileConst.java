/*
 *	ClassFileConst.java	1.2%	02/09/27 SMI
 *
 * Copyright (c) 1997,1999 Sun Microsystems, Inc. All Rights Reserved.
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

package util;

/*
 * Constants of the class file representation. This includes
 * magic numbers, signature representation, type numbers, access flags.
 * This DOES NOT include opcode numbers, which are, collectively,
 * represented in a VM-specific (including quicks), generated file.
 */

public interface ClassFileConst
{
    public static final char SIGC_VOID = 86;
    public static final String SIG_VOID = "V";
    public static final char SIGC_BOOLEAN = 90;
    public static final String SIG_BOOLEAN = "Z";
    public static final char SIGC_BYTE = 66;
    public static final String SIG_BYTE = "B";
    public static final char SIGC_CHAR = 67;
    public static final String SIG_CHAR = "C";
    public static final char SIGC_SHORT = 83;
    public static final String SIG_SHORT = "S";
    public static final char SIGC_INT = 73;
    public static final String SIG_INT = "I";
    public static final char SIGC_LONG = 74;
    public static final String SIG_LONG = "J";
    public static final char SIGC_FLOAT = 70;
    public static final String SIG_FLOAT = "F";
    public static final char SIGC_DOUBLE = 68;
    public static final String SIG_DOUBLE = "D";
    public static final char SIGC_ARRAY = 91;
    public static final String SIG_ARRAY = "[";
    public static final char SIGC_CLASS = 76;
    public static final String SIG_CLASS = "L";
    public static final char SIGC_METHOD = 40;
    public static final String SIG_METHOD = "(";
    public static final char SIGC_ENDCLASS = 59;
    public static final String SIG_ENDCLASS = ";";
    public static final char SIGC_ENDMETHOD = 41;
    public static final String SIG_ENDMETHOD = ")";
    public static final char SIGC_PACKAGE = 47;
    public static final String SIG_PACKAGE = "/";
    public static final int JAVA_MAGIC = 0xcafebabe;
    public static final int JAVA_VERSION = 45;
    public static final int JAVA_MINOR_VERSION = 3;
    public static final int CONSTANT_UTF8 = 1;
    public static final int CONSTANT_UNICODE = 2;
    public static final int CONSTANT_INTEGER = 3;
    public static final int CONSTANT_FLOAT = 4;
    public static final int CONSTANT_LONG = 5;
    public static final int CONSTANT_DOUBLE = 6;
    public static final int CONSTANT_CLASS = 7;
    public static final int CONSTANT_STRING = 8;
    public static final int CONSTANT_FIELD = 9;
    public static final int CONSTANT_METHOD = 10;
    public static final int CONSTANT_INTERFACEMETHOD = 11;
    public static final int CONSTANT_NAMEANDTYPE = 12;
    public static final int ACC_PUBLIC = 1;
    public static final int ACC_PRIVATE = 2;
    public static final int ACC_PROTECTED = 4;
    public static final int ACC_STATIC = 8;
    public static final int ACC_FINAL = 16;
    public static final int ACC_SYNCHRONIZED = 32;
    public static final int ACC_VOLATILE = 64;
    public static final int ACC_TRANSIENT = 128;
    public static final int ACC_NATIVE = 256;
    public static final int ACC_INTERFACE = 512;
    public static final int ACC_ABSTRACT = 1024;
    public static final int ACC_SUPER = 32;
    public static final int T_CLASS = 2;
    public static final int T_BOOLEAN = 4;
    public static final int T_CHAR = 5;
    public static final int T_FLOAT = 6;
    public static final int T_DOUBLE = 7;
    public static final int T_BYTE = 8;
    public static final int T_SHORT = 9;
    public static final int T_INT = 10;
    public static final int T_LONG = 11;
    public static final int T_VOID = 021;

}
