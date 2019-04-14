/*
 *  Copyright (c) 1999-2001 Sun Microsystems, Inc., 901 San Antonio Road,
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

package com.sun.cldc.io.j2me.resource;

import java.io.*;
import java.util.*;
import javax.microedition.io.*;
import com.sun.cldc.io.*;

/**
 * This class implements the default "resource:" protocol for KVM.
 *
 * The default is to open a file based upon the resource name.
 *
 * @author  Nik Shaylor
 * @version 1.0 2/12/2000
 */
public class Protocol extends ConnectionBase 
                      implements InputConnection {

    private String name;

    /**
     * Open the connection
     */
    public void open(String name, int mode, boolean timeouts)
        throws IOException {
        throw new RuntimeException("Should not be called");
    }

    /**
     * Open the connection
     */
    public Connection openPrim(String name, int mode, boolean timeouts)
        throws IOException {
        this.name = name;
        return this;
    }

    /**
     * Returns an input stream for a database record
     *
     * @return     an input stream for reading bytes from this record.
     * @exception  IOException  if an I/O error occurs when creating
     *                          the input stream.
     */
    public InputStream openInputStream() throws IOException {
        return new PrivateInputStream(name);
    }

    public void close() throws IOException {
    }
}

/**
 * Input stream class for the above connection
 */
class PrivateInputStream extends InputStream {

    // This field must be an Object, since it will point to 
    // something that needs to be protected from the garbage
    // collector.
    private Object handle;
    private int pos;
    private int count;

    /**
     * Constructor
     *
     * @exception  IOException  if an I/O error occurs.
     */
    public PrivateInputStream(String name) throws IOException {
        if (handle != null) {
            throw new IOException();
        }
        handle = open(name);
        count = available(handle);
        pos = 0;
    }

    /**
     * Reads the next byte of data from the input stream.
     *
     * @return     the next byte of data, or <code>-1</code>
     *             if the end of the stream is reached.
     * @exception  IOException  if an I/O error occurs.
     */
    public int read() throws IOException {
        int result;
        if ((result = read(handle)) != -1)
            pos++;
        return result;
    }

    public void close() throws IOException {
        close(handle);
        handle = null;
    }

    public int available() throws IOException {
        return count - pos;
    }

    public int read(byte b[], int off, int len) throws IOException {
        if (b == null) {
            throw new NullPointerException();
        } else if ((off < 0) || (off > b.length) || (len < 0) ||
                   ((off + len) > b.length) || ((off + len) < 0)) {
            throw new IndexOutOfBoundsException();
        } else if (len == 0) {
            return 0;
        }
        int readLength =  readBytes(handle, b, off, pos, len);
        if (readLength != -1)
            pos += readLength;
        return (readLength);
    }

    private static native Object open(String name) throws IOException;
    private static native int read(Object handle) throws IOException;
    private static native void close(Object handle) throws IOException;
    private static native int readBytes(Object handle, byte[] b, int offset, int pos, int len) throws IOException;
    private static native int available(Object handle) throws IOException;
}


