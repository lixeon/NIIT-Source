/*
 *	InterfaceMethodTable.java	1.7	98/07/22 SMI
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

package vm;
import java.util.Hashtable;
import java.util.Enumeration;
import components.ClassInfo;
import components.MethodInfo;
import components.MethodConstant;
import vm.Const;

/*
 * This class will build
 * the imethodtable, used by JDK1.1 for a quicker implementation
 * of invokeinterface_quick.
 * 
 * This class should be subclassed in a target-machine-specific way
 * to provide writing out the data structure we build here. 
 * See also InterfaceMethodFactory for the mechanism used to call back
 * into that target-specific body of code.
 * 
 * Each ClassInfo is already decorated with a vector of
 * all the interfaces it implements. As an extra hack, each
 * ClassInfo's allInterfaces vector has its parent's vector as prefix.
 * So:
 * A) a class which implements the same number of interfaces as its parent
 *    necessarily implements the same set of interfaces, and can use its
 *    parent's table, by reference.
 * B) for those classes for which a table must indeed be built,
 *    Only the interface elements associated with the present
 *    ClassInfo need to have a method index table built.
 */
public abstract
class InterfaceMethodTable {

    public ClassClass	   parent;
    public String	   name;
    public InterfaceVector iv[];

    protected InterfaceMethodTable( ClassClass c, String n, InterfaceVector v[] ){
	parent = c;
	name = n;
	iv = v;
    }

    private static MethodInfo findSlot( MethodConstant t[], MethodInfo m ){
	if ( t != null ){
	    MethodInfo v;
	    int nt = t.length;
	    int ID = m.getID();
	    for( int i = 0; i < nt; i++ ){
		if ((v=t[i].find()).getID() == ID )
		    return v;
	    }
	}
	return null;
    }

    /*
     * Iterate through the methods of an interface.
     * For the non-static members (i.e. the non-<clinit> )
     * assign a zero-based index, and use the methodTableIndex field
     * to keep it in.
     */
    public void 
    indexInterfaceMethods( ClassInfo c ){
	if ( (c.access&Const.ACC_INTERFACE) == 0 ){
	    System.err.println("InterfaceMethodTable.indexInterfaceMethods: "+c.className
		+" is not an interface");
	    return; // this is bad
	}
	int j = 0;
	MethodInfo m[] = c.methods;
	if ( m == null ) return; // no methods here.
	int n = m.length;
	for( int i =0; i < n; i++ ){
	    MethodInfo t = m[i];
	    if ( (t.access&Const.ACC_STATIC) != 0 )
		continue; // don't number statics.
	    if ( (t.access&Const.ACC_ABSTRACT) == 0 ){
		System.err.println("InterfaceMethodTable.indexInterfaceMethods: "+t
		+" is not abstract");
		// but keep going anyway...
	    }
	    t.methodTableIndex = j++;
	}
    }

    private static InterfaceMethodTable
    generateTablesForInterface( ClassInfo c, String imtName, InterfaceMethodFactory imf ){
	//
	// We generate a thing like an imethodtable for interfaces, too.
	// But its different enough that I'm treating it separately, here.
	//
	// There is no way to share our imethotable with our superclass,
	// as it must at least include ourselves!
	//
	// struct imethodtable { 
	//     int icount;
	//     struct { 
	// 	ClassClass *classdescriptor;
	// 	unsigned long *offsets = 0;
	//     } itable[1];
	// }
	ClassClass me = c.vmClass;
	int ntotal = c.allInterfaces.size();
	InterfaceVector ivec[] = new InterfaceVector[ ntotal+1 ];

	// first the entry for ourselves.
	ivec[ 0 ] = new InterfaceVector( me, c, null );
	// now all the others (if any)
	for( int i = 0; i< ntotal; i++){
	    ClassInfo intf = (ClassInfo)c.allInterfaces.elementAt( i );
	    ivec[ i+1 ] = new InterfaceVector( me, intf, null );
	}
	return imf.newInterfaceMethodTable( me, imtName, ivec );
    }


    public static InterfaceMethodTable
    generateInterfaceTable( ClassClass cc, InterfaceMethodFactory imf ){
	ClassInfo c = cc.ci;
	if ( c.allInterfaces == null )
	    c.findAllInterfaces();
	String imtName = c.getGenericNativeName() + "_intfMethodtable";
	ClassInfo sup = c.superClassInfo;
	int ntotal = c.allInterfaces.size();
	int nsuper = 0;

	if ( (c.access&Const.ACC_INTERFACE) != 0 ){
	    return generateTablesForInterface( c, imtName, imf );
	}
	if ( sup != null ){
	    nsuper = sup.allInterfaces.size();
	    if ( nsuper == ntotal ){
		// use other class's tables entirely.
		// we have nothing further to add.
		return sup.vmClass.inf;
	    }
	}
	//
	// generate the offset tables, or symbolic references
	// to them.
	InterfaceVector vec[] = new InterfaceVector[ ntotal ];
	if ( nsuper != 0 ){
	    // borrow some from superclass. They are the same.
	    System.arraycopy( sup.vmClass.inf.iv, 0, vec, 0, nsuper );
	}

	// compute the rest of the thing ourselves.
	for( int i = nsuper; i < ntotal; i++ ){
	    ClassInfo intf = (ClassInfo)c.allInterfaces.elementAt( i );
	    MethodConstant mtab[] = intf.refMethodtable;
	    int nmethods = mtab.length;
	    short ivec[] = new short[ nmethods ];
	    for( int j = 0; j < nmethods; j++ ){
		MethodInfo target = mtab[j].find();
		if ( (target.access&Const.ACC_STATIC) != 0 ){
		    // should never happen.
		    System.err.println("Interface "+intf.className+" has a static method in its methodtable:! "+target);
		    continue;
		}
		MethodInfo v = findSlot( c.refMethodtable, target );
		if ( v == null ){
		    System.err.println("Class "+c.className+" does not implement interface "+intf.className+" because it lacks "+target);
		    ivec[ j ] = 0;
		} else {
		    ivec[ j ] = (short)v.methodTableIndex;
		}
	    }
	    vec[ i ] = new InterfaceVector( cc, intf, ivec );
	}

	return imf.newInterfaceMethodTable( cc, imtName, vec );
    }
}
