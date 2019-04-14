/*
 * Copyright 1995-2002 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the proprietary information of Sun Microsystems, Inc.
 * Use is subject to license terms.
 *
 */

package java.lang;

/**
 * Thrown when a thread is waiting, sleeping, or otherwise paused for
 * a long time and another thread interrupts it using the
 * <code>interrupt</code>  method in class <code>Thread</code>.
 *
 * @author  Frank Yellin
 * @version 1.11, 12/04/99 (CLDC 1.0, Spring 2000)
 * @see     java.lang.Object#wait()
 * @see     java.lang.Object#wait(long)
 * @see     java.lang.Object#wait(long, int)
 * @see     java.lang.Thread#sleep(long)
 * @since   JDK1.0, CLDC 1.0
 */
public
class InterruptedException extends Exception {
    /**
     * Constructs an <code>InterruptedException</code> with no detail  message.
     */
    public InterruptedException() {
        super();
    }

    /**
     * Constructs an <code>InterruptedException</code> with the
     * specified detail message.
     *
     * @param   s   the detail message.
     */
    public InterruptedException(String s) {
        super(s);
    }
}


