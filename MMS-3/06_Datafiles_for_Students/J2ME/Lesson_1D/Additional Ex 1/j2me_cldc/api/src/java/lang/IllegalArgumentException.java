/*
 * Copyright 1994-2002 Sun Microsystems, Inc. All Rights Reserved.
 * 
 * This software is the proprietary information of Sun Microsystems, Inc.  
 * Use is subject to license terms.
 * 
 */

package java.lang;

/**
 * Thrown to indicate that a method has been passed an illegal or 
 * inappropriate argument.
 *
 * @author  unascribed
 * @version 1.16, 12/04/99 (CLDC 1.0, Sprint 2000)
 * @see	    java.lang.Thread#setPriority(int)
 * @since   JDK1.0, CLDC 1.0
 */
public
class IllegalArgumentException extends RuntimeException {
    /**
     * Constructs an <code>IllegalArgumentException</code> with no 
     * detail message. 
     */
    public IllegalArgumentException() {
	super();
    }

    /**
     * Constructs an <code>IllegalArgumentException</code> with the 
     * specified detail message. 
     *
     * @param   s   the detail message.
     */
    public IllegalArgumentException(String s) {
	super(s);
    }
}


