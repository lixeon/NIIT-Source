/*
 *	UninterpretedAttributeFactory.java	1.2	02/09/27
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
import java.io.IOException;

class UninterpretedAttributeFactory extends AttributeFactory {
    public static AttributeFactory instance = new UninterpretedAttributeFactory();

    Attribute finishReadAttribute(
	DataInput in, UnicodeConstant name, ConstantObject locals[], ConstantObject globals[] )
	throws IOException{
	    return UninterpretedAttribute.finishReadAttribute( in, name );
    }
}
