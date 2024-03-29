/*
 *	UninterpretedAttribute.java	1.7	02/09/27
 *
 * Copyright (c) 1997 Sun Microsystems, Inc. All Rights Reserved.
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
 * Use is subject to license terms.
 */

package components;
import java.io.DataOutput;
import java.io.DataInput;
import java.io.IOException;
import util.*;

/*
 * A class to represent the general Attribute form
 * in its most uninterpreted form. "data" is just an
 * array of byte. It can also be other things, even
 * array of ConstantObject.
 */

public
class UninterpretedAttribute extends Attribute
{
    public byte		data[];

    public
    UninterpretedAttribute( UnicodeConstant name, int l, byte d[] ){
	super( name, l );
	data = d;
    }

    protected int
    writeData( DataOutput o ) throws IOException{
	int trueLength = 0;
	o.write( data, 0, data.length );
	return data.length;
    }

    /*
     * For manipulating byte data 
     */
    public int getInt( int w ){
	return	(  (int)data[w]   << 24 ) |
		(( (int)data[w+1] &0xff ) << 16 ) |
		(( (int)data[w+2] &0xff ) << 8 ) |
		 ( (int)data[w+3] &0xff );
    }

    public int getUnsignedShort( int w ){
	return	(( (int)data[w] &0xff ) << 8 ) | ( (int)data[w+1] &0xff );
    }

    private void putShort( int w, short v ){
	data[w]   = (byte)(v>>>8);
	data[w+1] = (byte)v;
    }


    public static UninterpretedAttribute
    readAttribute( DataInput i, ConstantObject globals[] ) throws IOException{
	UnicodeConstant name;
	int l;
	byte d[];

	name = (UnicodeConstant)globals[i.readUnsignedShort()];
	l  = i.readInt();
	d = new byte[ l ];
	i.readFully( d );
	return new UninterpretedAttribute( name, l, d );
    }

    //
    // for those cases where we already read the name index
    // and know that its not something requiring special handling.
    //
    public static Attribute
    finishReadAttribute( DataInput i, UnicodeConstant name ) throws IOException {
	int l;
	byte d[];

	l  = i.readInt();
	d = new byte[ l ];
	i.readFully( d );
	return new UninterpretedAttribute( name, l, d );
    }

    public static Attribute[] 
    readAttributes( DataInput i, ConstantObject t[], boolean verbose ) throws IOException {
	int nattr =  i.readUnsignedShort();
	if ( verbose ){
	    System.out.println(Localizer.getString("uninterpretedattribute.reading_attributes", Integer.toString(nattr)));
	}
	if ( nattr == 0 ) return null;
	Attribute a[] = new Attribute[nattr];
	for ( int j = 0; j < nattr; j++ ){
	    a[j] = readAttribute( i, t );
	}
	return a;
    }

}
