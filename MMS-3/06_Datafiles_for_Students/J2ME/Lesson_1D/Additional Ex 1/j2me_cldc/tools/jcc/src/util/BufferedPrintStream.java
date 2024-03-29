/*
 *	BufferedPrintStream.java	1.6	99/02/11 SMI
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

package util;

import java.io.PrintStream;
import java.io.OutputStream;
import java.io.IOException;

/**
 * Like PrintStream, but does its own
 * buffering. This mashing of abstraction layers
 * saves many a methodcall.
 *
 * NOTE the lack of synchronization!
 * Also note the lack of autoflush.
 * Finally, note especially troublesome trick we use in determining
 * when to flush buffers. This relies on relative speed of dealing
 * with a Java exception and the size of the buffer, thus infrequency
 * of its occurance.
 */
public class BufferedPrintStream extends PrintStream {
    private final static int bufsize = 2*4096; // to choose a number at random.
    private final static int buflastindex = bufsize-1;
    protected byte[] buf;
    protected int curindex;
    protected OutputStream f;
    protected boolean errorOccured;
    protected IOException recentException;

    private final static byte NL = (byte)'\n';

    protected void flushit(){
	try{
	    f.write( buf, 0, curindex );
	} catch (IOException e ){
	    errorOccured = true;
	    recentException = e;
	}
	curindex = 0;
    }

    public BufferedPrintStream( OutputStream file ){
	super( file );// whatever...
	f = file;
	errorOccured = false;
	buf = new byte[ bufsize ];
	curindex = 0;
    }

    public boolean checkException(){
	flush();
	return errorOccured;
    }

    public void flush(){
	try{
	    if ( curindex != 0 ){
		f.write( buf, 0, curindex );
	    }
	    f.flush();
	} catch (IOException e ){
	    errorOccured = true;
	    recentException = e;
	}
	curindex = 0;
    }

    public void close(){
	flush();
	buf = null;
	numbuf = null;
	try {
	    if ( f != null )
		f.close();
	}catch( IOException e ){
	    errorOccured = true;
	    recentException = e;
	}
	super.close();
	f = null;
    }

    public void write( int b ){
	try {
	    buf[curindex] = (byte)b;
	    curindex++;
	    return;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = (byte)b;
	}
    }

    public void write( byte b[], int off, int len ){
	try{
	    if (len == 1 ){
		buf[curindex] = b[off];
		curindex++;
		return;
	    } else {
		System.arraycopy( b, off, buf, curindex, len );
		curindex += len;
		return;
	    }
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    System.arraycopy( b, off, buf, curindex, len );
	    curindex += len;
	}
    }

    private static byte[] truth = { (byte)'t', (byte)'r', (byte)'u', (byte)'e', (byte)'\n' };
    private static byte[] falsity= { (byte)'f', (byte)'a', (byte)'l', (byte)'s', (byte)'e', (byte)'\n' };

    public void print( boolean b ){
	if ( b ){
	    write( truth, 0, 4 );
	} else {
	    write( falsity, 0, 5 );
	}
    }

    public void println( boolean b ){
	if ( b ){
	    write( truth, 0, 5 );
	} else {
	    write( falsity, 0, 6 );
	}
    }

    public void print( char c ){
	try{
	    buf[curindex] = (byte)c;
	    curindex++;
	    return;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = (byte)c;
	}
    }

    public void println( char c ){
	try {
	    buf[curindex] = (byte)c;
	    curindex++;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = (byte)c;
	    buf[curindex++] = NL;
	    return;
	}
	try{
	    buf[curindex] = NL;
	    curindex++;
	    return;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = NL;
	}
    }

    public void print( char s[] ){
	int slen = s.length;
	for ( int i = 0; i < slen; i++ ){
	    try{
		buf[curindex] = (byte) s[i];
		curindex++;
		continue;
	    }catch( IndexOutOfBoundsException e ){
		flushit();
		buf[curindex++] = (byte) s[i];
	    }
	}
    }

    public void println( char s[] ){
	// enough pain to not be worth replicating the above
	print( s );
	//if ( curindex >= bufsize-1 ) flushit();
	// count on print to have left one space at end of buf for us.
	try{
	    buf[curindex] = NL;
	    curindex++;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = NL;
	}
    }

    private static final int nnumbuf = 25;
    private byte[] numbuf = new byte[nnumbuf]; // for integer conversions

    public void print( int i){
	if ( i < 0 ){
	    if ( i == Integer.MIN_VALUE ){
		// special case, not worth bothering with.
		print( Integer.toString(Integer.MIN_VALUE));
		return;
	    }
	    i = -i;
	    // since even negative numbers hardly happen,
	    // we don't mind this extra call...
	    write('-');
	}
	int n = nnumbuf;
	do {
	   numbuf[--n]= (byte)('0'+(i%10));
	   i /= 10;
	}while( i != 0 );
	int nsz = nnumbuf-n;
	try{
	    System.arraycopy( numbuf, n, buf, curindex, nsz );
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    System.arraycopy( numbuf, n, buf, curindex, nsz );
	}
	curindex += nsz;
    }

    public void println( int i ){
	// enough pain to not be worth replicating the above
	print( i );
	try{
	    buf[curindex] = NL;
	    curindex++;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = NL;
	}
    }

    public void print( long l){
	if ( l < 0 ){
	    if ( l == Long.MIN_VALUE ){
		// special case, not worth bothering with.
		print( Long.toString(Long.MIN_VALUE));
		return;
	    }
	    l = -l;
	    // since even negative numbers hardly happen,
	    // we don't mind this extra call...
	    write('-');
	}
	int n = nnumbuf;
	do {
	   numbuf[--n]= (byte)('0'+(byte)(l%10L));
	   l /= 10L;
	}while( l != 0L );
	int nsz = nnumbuf-n;
	try{
	    System.arraycopy( numbuf, n, buf, curindex, nsz-n );
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    System.arraycopy( numbuf, n, buf, curindex, nsz-n );
	}
	curindex += nsz;
    }

    public void println( long l ){
	// enough pain to not be worth replicating the above
	print( l );
	try{
	    buf[curindex] = NL;
	    curindex++;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = NL;
	}
    }

    public void print( Object o ){
	if ( o == null )
	    print( "(null)");
	else
	    print( o.toString() );
    }

    public void println( Object o ){
	if ( o == null )
	    println( "(null)");
	else
	    println( o.toString() );
    }

    public void print( String s ){
	int slen = s.length();
	try{
	    s.getBytes( 0, slen, buf, curindex );
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    s.getBytes( 0, slen, buf, curindex );
	}
	curindex += slen;
    }

    public void println( String s ){
	int slen = s.length();
	try{
	    s.getBytes( 0, slen, buf, curindex );
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    s.getBytes( 0, slen, buf, curindex );
	}
	curindex += slen;
	try{
	    buf[curindex] = NL;
	    curindex++;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = NL;
	}
    }

    public void println(){
	try{
	    buf[curindex] = NL;
	    curindex++;
	}catch( IndexOutOfBoundsException e ){
	    flushit();
	    buf[curindex++] = NL;
	}
    }

}
