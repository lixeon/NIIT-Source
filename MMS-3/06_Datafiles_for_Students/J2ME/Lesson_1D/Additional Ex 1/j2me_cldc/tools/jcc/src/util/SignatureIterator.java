/*
 *	SignatureIterator.java	1.2	99/04/06 SMI
 *
 * Copyright (c) 1998 Sun Microsystems, Inc. All Rights Reserved.
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

package util;
import vm.Const;

public abstract class SignatureIterator {
    
    protected String sig;
    private int	   sigStart;
    private int	   cur;
    private int	   end;
    private int	   arrayDepth;

    public  boolean isSingleton;
    protected boolean isReturnType;


    public SignatureIterator( String sig, int start, int end ){
	this.sig = sig;
	this.sigStart = cur = start;
	this.end = end;
	isSingleton = sig.charAt( start ) != Const.SIGC_METHOD;
    }

    public SignatureIterator( String sig ){
	this( sig, 0, sig.length()-1);
    }

    /*
     * Override any of these to do something more intelligent
     */
    public void do_boolean(){
	do_scalar( Const.SIGC_BOOLEAN );
    }
    public void do_char  (){
	do_scalar( Const.SIGC_CHAR );
    }
    public void do_float (){
	do_scalar( Const.SIGC_FLOAT );
    }
    public void do_double(){
	do_scalar( Const.SIGC_DOUBLE );
    }
    public void do_byte  (){
	do_scalar( Const.SIGC_BYTE );
    }
    public void do_short (){
	do_scalar( Const.SIGC_SHORT );
    }
    public void do_int   (){
	do_scalar( Const.SIGC_INT );
    }
    public void do_long  (){
	do_scalar( Const.SIGC_LONG );
    }
    public void do_void  (){
	do_scalar( Const.SIGC_VOID );
    }

    public void do_scalar( char c ){
	return;
    }

    //
    // do_array and do_object are expected to parse the subtype
    // information themselves.
    // if its a simple type (do_array only),
    // then subTypeEnd == subTypeStart
    // else  sig.charAt(subTypeStart) == Const.SIGC_CLASS 
    // and   sig.charAt(subTypeEnd)   == Const.SIGC_ENDCLASS
    //
    public abstract void do_array( int arrayDepth, int subTypeStart, int subTypeEnd );
    public abstract void do_object( int subTypeStart, int subTypeEnd );

    public boolean isMethod(){ return !isSingleton; }

    public void iterate_parameters() throws DataFormatException {
	cur = sigStart;
	if ( isSingleton )
	    throw new DataFormatException(sig.substring(cur-1, end)+" is not a method signature");
	cur += 1;
	isReturnType = false;
	char c;
	while ((c=sig.charAt(cur++) ) != Const.SIGC_ENDMETHOD ){
	    if ( cur > end ){
		throw new DataFormatException( "unterminated parameter list");
	    }
	    next( c );
	}
    }

    public void iterate_returntype() throws DataFormatException {
	if ( isSingleton )
	    throw new DataFormatException(sig.substring(sigStart, end)+" is not a method signature");
	int endpos = sig.indexOf( Const.SIGC_ENDMETHOD, sigStart );
	isReturnType = true;
	if ( (endpos < 0 ) || (endpos > end ) )
	    throw new DataFormatException( "unterminated parameter list");
	cur = endpos+1;
	next( sig.charAt(cur++) );
    }

    public void iterate() throws DataFormatException {
	if ( isSingleton ){
	    // trivial case.
	    isReturnType = false;
	    cur = sigStart;
	    next( sig.charAt(cur++) );
	} else {
	    iterate_parameters();
	    iterate_returntype();
	}
    }

    private int
    delimitClassname( ) throws DataFormatException{
	int endpos = sig.indexOf( Const.SIGC_ENDCLASS, cur );
	if ( (endpos < 0 ) || (endpos > end ) )
	    throw new DataFormatException( "unending class name");
	return endpos;
    }

    private void next( char c ) throws DataFormatException {
	switch (c){
	case Const.SIGC_BOOLEAN:
	    do_boolean(); return;
	case Const.SIGC_BYTE:
	    do_byte(); return;
	case Const.SIGC_CHAR:
	    do_char(); return;
	case Const.SIGC_SHORT:
	    do_short(); return;
	case Const.SIGC_INT:
	    do_int(); return;
	case Const.SIGC_FLOAT:
	    do_float(); return;
	case Const.SIGC_LONG:
	    do_long(); return;
	case Const.SIGC_DOUBLE:
	    do_double(); return;
	case Const.SIGC_VOID:
	    do_void();
	    return;

	case Const.SIGC_CLASS:
	    int startClassName = cur-1;
	    int endClassName = delimitClassname( );
	    cur = endClassName+1;
	    do_object( startClassName, endClassName );
	    return;

	case Const.SIGC_ARRAY:
	    arrayDepth = 0;
	    cur -= 1;
	    do {
		arrayDepth += 1;
	    }while ((cur<=end) && ((c=sig.charAt(++cur)) == Const.SIGC_ARRAY));
	    if ( cur > end )
		throw new DataFormatException( "array of nothing");
	    int startBaseType = cur;
	    int endBaseType;
	    // now isolate at the base type.
	    if ( c == Const.SIGC_CLASS ){
		endBaseType = delimitClassname( );
	    } else {
		endBaseType = startBaseType;
	    }
	    cur = endBaseType+1;
	    do_array( arrayDepth, startBaseType, endBaseType );
	    return;

	case Const.SIGC_METHOD:
	    // cannot happen here!
	default:
	    throw new DataFormatException( "unparseable signature: " + sig);
	}
    }

    // when the baseType is a class, we have the name stashed away.
    public String className( int startpos, int endpos){
	return sig.substring( startpos+1, endpos );
    }

    public static boolean isPrimitiveType( String sig ){
	char c = sig.charAt(0);
	return ( (c!=Const.SIGC_METHOD) && (c!=Const.SIGC_ARRAY) && (c!=Const.SIGC_CLASS) );
    }
}
