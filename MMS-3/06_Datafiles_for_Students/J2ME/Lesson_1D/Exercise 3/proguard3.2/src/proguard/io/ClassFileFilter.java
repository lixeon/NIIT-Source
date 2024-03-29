/* $Id: ClassFileFilter.java,v 1.2 2004/08/15 12:39:30 eric Exp $
 *
 * ProGuard -- shrinking, optimization, and obfuscation of Java class files.
 *
 * Copyright (c) 2002-2004 Eric Lafortune (eric@graphics.cornell.edu)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
package proguard.io;

import proguard.classfile.*;
import proguard.util.*;

import java.io.*;


/**
 * This DataEntryReader delegates to one of two other DataEntryReader instances,
 * depending on the extension of the data entry.
 *
 * @author Eric Lafortune
 */
public class ClassFileFilter implements DataEntryReader
{
    private FilteredDataEntryReader filteredDataEntryReader;


    /**
     * Creates a new ClassFileFilter that delegates reading class files to the
     * given reader.
     */
    public ClassFileFilter(DataEntryReader classFileReader)
    {
        this(classFileReader, null);
    }


    /**
     * Creates a new ClassFileFilter that delegates to either of the two given
     * readers.
     */
    public ClassFileFilter(DataEntryReader classFileReader,
                           DataEntryReader dataEntryReader)
    {
        filteredDataEntryReader =
            new FilteredDataEntryReader(
            new DataEntryNameFilter(
            new ExtensionMatcher(ClassConstants.CLASS_FILE_EXTENSION)),
            classFileReader,
            dataEntryReader);
    }


    // Implementations for DataEntryReader.

    public void read(DataEntry dataEntry) throws IOException
    {
        filteredDataEntryReader.read(dataEntry);
    }
}
