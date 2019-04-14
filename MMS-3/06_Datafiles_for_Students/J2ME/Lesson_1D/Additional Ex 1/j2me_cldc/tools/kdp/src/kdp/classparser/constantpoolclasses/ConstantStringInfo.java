/*
 *  Copyright (c) 2000 Sun Microsystems, Inc., 901 San Antonio Road,
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
package kdp.classparser.constantpoolclasses;

import java.io.*;

/**
 * Encapsulates a CONSTANT_String item a Java class file 
 * constant pool. 
 *
 * @author 			Aaron Dietrich
 * @version			$Id: ConstantStringInfo.java,v 1.1.1.1 2000/07/07 13:34:24 jrv Exp $
 *
 * Revision History
 *   $Log: ConstantStringInfo.java,v $
 *   Revision 1.1.1.1  2000/07/07 13:34:24  jrv
 *   Initial import of kdp code
 *
 *   Revision 1.1.1.1  2000/05/31 19:14:48  ritsun
 *   Initial import of kvmdt to CVS
 *
 *   Revision 1.1  2000/04/25 00:34:06  ritsun
 *   Initial revision
 *
 */
public class ConstantStringInfo extends ConstantPoolInfo
  {
   /** index into constant pool containing a UTF8Info structure */
   private int		stringIndex;

   /**
    * Constructor.  Creates a ConstantStringInfo object.
    *
    * @param        iStream        input stream to read from
    *
    * @exception    IOException    just pass IOExceptions up
    */
   public ConstantStringInfo (DataInputStream iStream) throws IOException
     {
      tag = ConstantPoolInfo.CONSTANT_String;
      
      stringIndex = iStream.readUnsignedShort ();
     }

   /**
    * Returns this ConstantStringInfo's information as a string.
    *
    * @return       String         info as a string
    */
   public String toString ()
     {
      return ("CONSTANT_String\t" + "stringIndex=\t" + stringIndex);
     }

   /**
    * Returns the string referenced by this ConstantStringInfo object
    * from the constant pool.
    *
    * @param        constantPool   class's constant pool
    *
    * @return       String         referenced string
    */
   public String toString (final ConstantPoolInfo[] constantPool)
     {
      ConstantUtf8Info        utf8Info = (ConstantUtf8Info) constantPool[stringIndex];

      return ("CONSTANT_String=\t" + utf8Info.toString ());
     }
  }
