/*
 *	EVMClass.java	1.10	99/05/11	SMI
 *
 * Copyright (c) 1999 Sun Microsystems, Inc. All Rights Reserved.
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

package vm;

/*
 * EVM-specific internal representation of
 * a class. Target-machine independent.
 * There is a references from each instance of components.ClassInfo to 
 * one of these, and a reference back as well.
 * Derived from the JDK-specific ClassClass
 *
 * !!See also EVMVM for VM-specific info not associated directly with a class.
 */
import components.*;
import util.*;
import vm.Const;
import vm.EVMConst;
import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;
import java.util.StringTokenizer;

public class
EVMClass extends ClassClass implements Const, EVMConst {
    public EVMMethodInfo	methods[];
    private String		myNativeName;
    protected int		typeCode = 0;

    public int			nStaticWords;
    public int			nStaticRef;
    public FieldInfo		statics[];

    public EVMClass( ClassInfo c ){
	ci = c;
	c.vmClass = this;

	if ( c.methods != null ){
	    // per-method, EVM-specific info.
	    methods = new EVMMethodInfo[ c.methods.length ];
	    for( int i = 0; i < methods.length; i++ ){
		components.MethodInfo m = c.methods[i];
		methods[i] = new EVMMethodInfo( m );
		if (!hasStaticInitializer &&
		    m.isStaticMember() &&
		    m.name.string.equals(/*NOI18N*/"<clinit>")) {
		    hasStaticInitializer = true;
		}
	    }
	}
    }

    public String getNativeName(){
	if ( myNativeName == null ){
	    if ( ci instanceof ArrayClassInfo ){
		ArrayClassInfo aci = (ArrayClassInfo)ci;
		if (aci.depth == 1) {
		    /*
		     * There are some special arrays of well-known basic
		     * types here.
		     */
		    if (aci.baseClass == null) {
			myNativeName = "manufacturedArrayOf"+
			    aci.baseName;
		    } else if (aci.baseClass.find().superClassInfo == null) {
			myNativeName = "manufacturedArrayOfObject";
		    } else {
		        myNativeName = "manufacturedArrayClass"+
			    aci.arrayClassNumber;
		    }
		} else {
		    myNativeName = "manufacturedArrayClass"+
			aci.arrayClassNumber;
		}
	    } else {
		myNativeName = ci.getGenericNativeName();
	    }
	}
	return myNativeName;
    }

    public int
    EVMflags(){
	int flagval = 0;
	int a = ci.access;
	if ( (a&ACC_PUBLIC) != 0 ) flagval |= EVM_CLASS_ACC_PUBLIC;
	if ( (a&ACC_FINAL) != 0 ) flagval |= EVM_CLASS_ACC_FINAL;
	if ( (a&ACC_SUPER) != 0 ) flagval |= EVM_CLASS_ACC_SUPER;
	if ( (a&ACC_INTERFACE) != 0 ) flagval |= EVM_CLASS_ACC_INTERFACE;
	if ( (a&ACC_ABSTRACT) != 0 ) flagval |= EVM_CLASS_ACC_ABSTRACT;
	return flagval;

    }

    public int
    typecode(){
	return typeCode;
    }

    public void orderStatics(){
	// count statics.
	// arranged them ref-first
	// do not assign offsets.
	FieldInfo f[] = ci.fields;
	nStaticWords = 0;
	if ( (f == null) || (f.length == 0 ) ) return; // nothing to do.
	int nFields = f.length;
	int nStatic = 0;
	int nRef = 0;
	for ( int i = 0; i < nFields; i++ ){
	    FieldInfo fld = f[i];
	    if ( fld.isStaticMember() ){
		nStatic += 1;
		char toptype =  fld.type.string.charAt(0);
		if ( (toptype == 'L') || (toptype=='['))
		    nRef += 1;
	    }
	}
	int refOff     = 0;
	int scalarOff = nRef;
	int totalStaticWords = nRef;
	statics = new FieldInfo[ nStatic ];
	for ( int i = 0; i < nFields; i++ ){
	    FieldInfo fld = f[i];
	    if ( fld.isStaticMember() ){
		char toptype =  fld.type.string.charAt(0);
		if ( (toptype == 'L') || (toptype=='[')){
		    statics[refOff] = fld;
		    refOff += 1;
		} else {
		    statics[scalarOff] = fld;
		    scalarOff += 1;
		    totalStaticWords += fld.nSlots;
		}
	    }
	}
	nStaticWords = totalStaticWords;
	nStaticRef   = nRef;
    }

}
