/*
 * Copyright 1995-2002 Sun Microsystems, Inc. All Rights Reserved.
 * 
 * This software is the proprietary information of Sun Microsystems, Inc.  
 * Use is subject to license terms.
 * 
 */

package java.io;

/**
 * Signals that a malformed UTF-8 string has been read in a data
 * input stream or by any class that implements the data input
 * interface. See the <code>writeUTF</code> method for the format in
 * which UTF-8 strings are read and written.
 *
 * @author  Frank Yellin
 * @version 1.8, 12/04/99 (CLDC 1.0, Spring 2000)
 * @see     java.io.DataInput
 * @see     java.io.DataInputStream#readUTF(java.io.DataInput)
 * @see     java.io.IOException
 * @since   JDK1.0, CLDC 1.0
 */
public
class UTFDataFormatException extends IOException {
    /**
     * Constructs a <code>UTFDataFormatException</code> with
     * <code>null</code> as its error detail message.
     */
    public UTFDataFormatException() {
	super();
    }

    /**
     * Constructs a <code>UTFDataFormatException</code> with the
     * specified detail message. The string <code>s</code> can be
     * retrieved later by the
     * <code>{@link java.lang.Throwable#getMessage}</code>
     * method of class <code>java.lang.Throwable</code>.
     *
     * @param   s   the detail message.
     */
    public UTFDataFormatException(String s) {
	super(s);
    }
}


