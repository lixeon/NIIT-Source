/*
 * @(#)Packet.java	1.25 99/05/21
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
 * Use is subject to license terms.
 */

package kdp;

import java.io.*;

public class Packet extends Object {
    public final static short NoFlags = 0x0;
    public final static short Reply = 0x80;
    public final static short ReplyNoError = 0x0;

    static int uID = 0x80000001;	/* start at -2 billion and go up */
    final static byte[] nullData = new byte[0];

    // Note! flags, cmdSet, and cmd are all byte values.
    // We represent them as shorts to make them easier
    // to work with. 
    int id;
    short flags;
    short cmdSet;
    short cmd;
    short errorCode;
    byte[] data;
    volatile boolean replied = false;
    int curReadIndex, curWriteIndex;

    Packet()
    {
        id = uniqID();
        flags = NoFlags;
        data = new byte[ 1024 ];
        curReadIndex = curWriteIndex = 0;
    }

    static synchronized private int uniqID()
    {
        /*
         * JDWP spec does not require this id to be sequential and 
         * increasing, but our implementation does. See 
         * VirtualMachine.notifySuspend, for example.
         */
        return uID++;
    }

    public int getLength() {
        return curWriteIndex;
    }
  
    public void setData( byte[] d ) {
        data = new byte[ d.length ];
        System.arraycopy( d, 0, data, 0, data.length );
        curWriteIndex = data.length;
        curReadIndex = 0;
    }
    
    public String toString() {
        //return cmdSet+"/"+cmd+"/"+errorCode+"\n--->\n"+new String( data )+"\n<---\n";
	StringBuffer s = new StringBuffer();
        s.append(cmdSet).append("/").append(cmd).append("/").append(errorCode).append("\n--->\n");
	for (int i = 0; i < data.length; i++) {
        s.append(Integer.toHexString((int)((long)data[i] & 0xFF))).append(".");
	}
        s.append("\n<----\n");
	return s.toString();
    }

}
