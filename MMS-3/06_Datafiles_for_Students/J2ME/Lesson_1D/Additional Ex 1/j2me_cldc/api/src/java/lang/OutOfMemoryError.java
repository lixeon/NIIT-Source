/*
 * Copyright 1994-2002 Sun Microsystems, Inc. All Rights Reserved.
 * 
 * This software is the proprietary information of Sun Microsystems, Inc.  
 * Use is subject to license terms.
 * 
 */

package java.lang;

/**
 * Thrown when the Java Virtual Machine cannot allocate an object 
 * because it is out of memory, and no more memory could be made 
 * available by the garbage collector. 
 *
 * @author  unascribed
 * @version 1.17, 12/04/99 (CLDC 1.0, Spring 2000)
 * @since   JDK1.0, CLDC 1.0
 */
public
class OutOfMemoryError extends VirtualMachineError {
    /**
     * Constructs an <code>OutOfMemoryError</code> with no detail message.
     */
    public OutOfMemoryError() {
	super();
    }

    /**
     * Constructs an <code>OutOfMemoryError</code> with the specified 
     * detail message. 
     *
     * @param   s   the detail message.
     */
    public OutOfMemoryError(String s) {
	super(s);
    }
}


