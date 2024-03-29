/*
 * @(#)ConnectionService.java	1.5 99/05/21
 * 
 * Copyright (c) 1997-1999 Sun Microsystems, Inc. All Rights Reserved.
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
 * 
 * 
 * Use is subject to license terms.
 */

package kdp;

import java.io.IOException;

/**
 * Connection SPI
 */
public interface ConnectionService {
    void close();
    byte receiveByte() throws IOException;
    void sendByte(byte b) throws IOException;

    // TO DO: Hacks for now; we will eventually have PacketService interfaces
    Packet receivePacket() throws IOException;
    void sendPacket(Packet p) throws IOException;
}

