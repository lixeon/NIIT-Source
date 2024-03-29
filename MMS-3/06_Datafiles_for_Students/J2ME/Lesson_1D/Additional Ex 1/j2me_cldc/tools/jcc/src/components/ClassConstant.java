/*
 *	ClassConstant.java	1.3	99/04/06 SMI
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
import util.DataFormatException;
import vm.Const;

// ClassConstant represents a CONSTANT_Class stored in a constant pool.

public
class ClassConstant extends ConstantObject
{
    public UnicodeConstant name;
    
    private boolean	 didLookup;
    private ClassInfo	 theClass;

    public int nameIndex;

    ClassConstant( int t, int n ){ 
	tag = t;
	nameIndex = n;
	nSlots = 1;
    }

    public ClassConstant( UnicodeConstant n ){
	tag = Const.CONSTANT_CLASS;
	name = n;
	nSlots = 1;
	resolved = true;
    }

    // Read from constant pool.
    public static
    ClassConstant read( int tag, DataInput in ) throws IOException {
	return new ClassConstant( tag, in.readUnsignedShort() );
    }

    public void
    resolve( ConstantObject table[] ){
	if (resolved) return;
	name = (UnicodeConstant)table[nameIndex];
	resolved = true;
    }

    public void externalize( ConstantPool p ){
	name = (UnicodeConstant)p.add( name );
    }

    // Write out reference to the ClassClass data structure
    public void write( DataOutput o ) throws IOException{
	o.writeByte( tag );
	if ( ! resolved )
	    throw new DataFormatException("unresolved ClassConstant");
	int x = name.index;
	if ( x == 0 )
	    throw new DataFormatException("0 name index for "+name.string);
	o.writeShort( x );
    }

    public String toString(){
	if ( resolved )
	    return "Class: "+name.toString();
	else
	    return "Class: ["+nameIndex+"]";
    }


    public int hashCode() {
	return tag + name.hashCode();
    }

    public void incReference() {
	references++;
	name.incReference();
    }

    public void decReference() {
	references--;
	name.decReference();
    }

    public boolean equals(Object o) {
	if (o instanceof ClassConstant) {
	    ClassConstant c = (ClassConstant) o;
	    return name.string.equals(c.name.string);
	} else {
	    return false;
	}
    }

    public ClassInfo find(){
	if ( !didLookup ){
	    theClass = ClassInfo.lookupClass( name.string );
	    didLookup = true;
	}
	return theClass; // which may be null
    }

    public void forget(){
	didLookup = false;
	theClass = null;
    }

    public boolean isResolved(){ return find() != null; }
}
