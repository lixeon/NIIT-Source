/*
 *  Copyright (c) 1999-2002 Sun Microsystems, Inc., 901 San Antonio Road,
 *  Palo Alto, CA 94303, U.S.A.  All Rights Reserved.
 *
 *  Sun Microsystems, Inc. has intellectual property rights relating
 *  to the technology embodied in this software.  In particular, and
 *  without limitation, these intellectual property rights may include
 *  one or more U.S. patents, foreign patents, or pending
 *  applications.  Sun, Sun Microsystems, the Sun logo, Java, KJava,
 *  and all Sun-based and Java-based marks are trademarks or
 *  registered trademarks of Sun Microsystems, Inc.  in the United
 *  States and other countries.
 *
 *  This software is distributed under licenses restricting its use,
 *  copying, distribution, and decompilation.  No part of this
 *  software may be reproduced in any form by any means without prior
 *  written authorization of Sun and its licensors, if any.
 *
 *  FEDERAL ACQUISITIONS:  Commercial Software -- Government Users
 *  Subject to Standard License Terms and Conditions
 */

package com.sun.cldc.i18n.j2me;

import java.io.*;
import com.sun.cldc.i18n.*;


/**
 * Default class reading input streams
 *
 * @author  Nik Shaylor, Antero Taivalsaari
 * @version 1.0 10/18/99
 * @version 1.1 03/29/02
 */

public class ISO8859_1_Reader extends StreamReader {

    private static int BBUF_LEN = 128;

    /**
     * Read a single character.
     *
     * @exception  IOException  If an I/O error occurs
     */
    synchronized public int read() throws IOException {
        return in.read();
    }

    /**
     * Read characters into a portion of an array.
     *
     * @exception  IOException  If an I/O error occurs
     */
    synchronized public int read(char cbuf[], int off, int len)
        throws IOException {

        // Allocate a private buffer to speed up reading
        int bbuflen = (len > BBUF_LEN) ? BBUF_LEN : len;
        byte bbuf[] = new byte[bbuflen];

        int count = 0;
        while (count < len) {
            int nbytes = len - count;
            if (nbytes > bbuflen) nbytes = bbuflen;
            nbytes = in.read(bbuf, 0, nbytes);

            if (nbytes == -1) {
                return (count == 0) ? -1 : count;
            }

            for (int i = 0; i < nbytes; i++) {
                cbuf[off++] = (char)(bbuf[i] & 0xFF);
            }

            count += nbytes;
        }
        return len;
    }

    /**
     * Get the size in chars of an array of bytes
     */
    public int sizeOf(byte[] array, int offset, int length) {
        return length;
    }

}


