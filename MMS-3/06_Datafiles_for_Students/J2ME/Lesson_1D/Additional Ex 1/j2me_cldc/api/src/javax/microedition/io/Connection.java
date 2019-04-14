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
 * This is the most basic type of generic connection. Only the close
 * method is defined. The open method is not defined here because opening is
 * always done using the Connector.open() methods.
 *
 * @author  Nik Shaylor
 * @version 1.1 1/25/2000
 * @since   CLDC 1.0
 */
public interface Connection {

    /**
     * Close the connection.
     * <p>
     * When a connection has been closed, access to any of its methods
     * except this close() will cause an IOException to be thrown.
     * Closing an already closed connection has no effect. Streams 
     * derived from the connection may be open when method is called.
     * Any open streams will cause the connection to be held open
     * until they themselves are closed. In this latter case access
     * to the open streams is permitted, but access to the connection
     * is not.
     *
     * @exception IOException  If an I/O error occurs
     */
    public void close() throws IOException;
}

