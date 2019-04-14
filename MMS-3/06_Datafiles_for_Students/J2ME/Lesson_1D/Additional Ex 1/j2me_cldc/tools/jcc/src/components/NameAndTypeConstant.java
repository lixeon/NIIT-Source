/*
 *	NameAndTypeConstant.java	1.3	99/04/06 SMI
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
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import vm.Const;
import util.DataFormatException;
// This class represents a CONSTANT_NameAndType

public
class NameAndTypeConstant extends ConstantObject {
    // Filled in by Clas.resolveConstants()
    public UnicodeConstant name;
    public UnicodeConstant type;

    // Read in from classfile
    int nameIndex;
    int typeIndex;

    // The unique ID for this name and type (from Std2ID)
    public int ID = 0;

    private NameAndTypeConstant( int t, int ni, int ti ){
	tag = t;
	nameIndex = ni;
	typeIndex = ti;
	nSlots = 1;
    }

    public NameAndTypeConstant( UnicodeConstant name, UnicodeConstant type ){
	tag = Const.CONSTANT_NAMEANDTYPE;
	this.name = name;
	this.type = type;
	nSlots = 1;
	resolved = true;
    }

    public static ConstantObject read( int t, DataInput i ) throws IOException{
	return new NameAndTypeConstant( t, i.readUnsignedShort(), i.readUnsignedShort() );
    }

    public void resolve( ConstantObject table[] ){
	if ( resolved ) return;
	name = (UnicodeConstant)table[nameIndex];
	type = (UnicodeConstant)table[typeIndex];
	resolved = true;
    }

    public void externalize( ConstantPool p ){
	name = (UnicodeConstant)p.add( name );
	type = (UnicodeConstant)p.add( type );
    }

    public void write( DataOutput o ) throws IOException{
	o.writeByte( tag );
	if ( resolved ){
	    o.writeShort( name.index );
	    o.writeShort( type.index );
	} else {
	    throw new DataFormatException("unresolved NameAndTypeConstant");
	    //o.writeShort( nameIndex );
	    //o.writeShort( typeIndex );
	}
    }

    public String toString(){
	if ( resolved ){
	    return "NameAndType: "+name.string+" : "+type.string;
	}else{
	    return "NameAndType[ "+nameIndex+" : "+typeIndex+" ]";
	}
    }

    public void incReference() {
	references++;
	name.incReference();
	type.incReference();
    }

    public void decReference() {
	references--;
	name.decReference();
	type.decReference();
    }


    public int hashCode() {
	return tag + name.string.hashCode() + type.string.hashCode();
    }
    

    public boolean equals(Object o) {
	if (o instanceof NameAndTypeConstant) {
	    NameAndTypeConstant n = (NameAndTypeConstant) o;
	    return name.string.equals(n.name.string) &&
		   type.string.equals(n.type.string);
	} else {
	    return false;
	}
    }

    public boolean isResolved(){ return true; }
}
