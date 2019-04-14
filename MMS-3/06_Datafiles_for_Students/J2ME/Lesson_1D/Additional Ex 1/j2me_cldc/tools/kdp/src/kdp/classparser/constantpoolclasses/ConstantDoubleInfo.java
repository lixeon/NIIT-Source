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
 * Encapsulates a CONSTANT_Double in a Java class file
 * constant pool.
 *
 * @author 			Aaron Dietrich
 * @version			$Id: ConstantDoubleInfo.java,v 1.1.1.1 2000/07/07 13:34:24 jrv Exp $
 *
 * Revision History
 *   $Log: ConstantDoubleInfo.java,v $
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
public class ConstantDoubleInfo extends ConstantPoolInfo
  {
   /** The high bytes of this double */
   private int		highBytes;
   /** The low bytes of this double */
   private int		lowBytes;
   
   /**
    * Constructor.  Reads in appropriate data.
    *
    * @param        iStream        input stream to read from
    *
    * @exception    IOException    just pass IOExceptions up
    */
   public ConstantDoubleInfo (DataInputStream iStream) throws IOException
     {
      tag = ConstantPoolInfo.CONSTANT_Double;

      highBytes = iStream.readInt ();
      lowBytes = iStream.readInt ();
     }

   /**
    * Returns the value of this double as a string by converting the
    * high and low bytes to a double using the method specified in the
    * JVM Specification.
    *
    * @return       String         The double as a string.
    */
   public String toString ()
     {
      long          longValue = ((long) highBytes << 32) + lowBytes;

      if (longValue == 0x7ff0000000000000L)
        return ("CONSTANT_Double=\t" + "Positive Infinity");

      if (longValue == 0xfff0000000000000L)
        return ("CONSTANT_Double=\t" + "Negative Infinity");

      if (((longValue >= 0x7ff0000000000001L) && (longValue <= 0x7fffffffffffffffL)) ||
          ((longValue >= 0xfff0000000000001L) && (longValue <= 0xffffffffffffffffL)))
        return ("CONSTANT_Double=\t" + "NaN");

      int s = ((longValue >> 63) == 0) ? 1 : -1;
      int e = (int)((longValue >> 52) & 0x7ffL);
      long m = (e == 0) ? (longValue & 0xfffffffffffffL) << 1 :
                          (longValue & 0xfffffffffffffL) | 0x10000000000000L;

      double value = s * m * (2^(e - 1075));

      return ("CONSTANT_Double=\t" + value);
     }
  }
