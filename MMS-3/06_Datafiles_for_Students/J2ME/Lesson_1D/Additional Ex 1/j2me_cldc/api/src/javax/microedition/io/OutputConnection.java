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

package javax.microedition.io;

import java.io.*;

/**
 * This interface defines the capabilities that an output 
 * stream connection must have.
 *
 * @author  Nik Shaylor
 * @version 1.1 1/7/2000
 * @since   CLDC 1.0
 */
public interface OutputConnection extends Connection {

    /**
     * Open and return an output stream for a connection.
     *
     * @return                 An output stream
     * @exception IOException  If an I/O error occurs
     */
    public OutputStream openOutputStream() throws IOException;

    /**
     * Open and return a data output stream for a connection.
     *
     * @return                 An output stream
     * @exception IOException  If an I/O error occurs
     */
    public DataOutputStream openDataOutputStream() throws IOException;
}


