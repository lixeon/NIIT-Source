/* $Id: FileWordReader.java,v 1.9 2004/08/15 12:39:30 eric Exp $
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
package proguard;

import java.io.*;
import java.net.URL;


/**
 * A <code>WordReader</code> that returns words from a file or a URL.
 *
 * @author Eric Lafortune
 */
public class FileWordReader extends WordReader
{
    private String           fileName;
    private LineNumberReader reader;


    /**
     * Creates a new FileWordReader for the given file name.
     */
    public FileWordReader(String fileName) throws IOException
    {
        this.fileName = fileName;
        this.reader   = new LineNumberReader(
                        new BufferedReader(
                        new FileReader(fileName)));
    }


    /**
     * Creates a new FileWordReader for the given URL.
     */
    public FileWordReader(URL url) throws IOException
    {
        this.fileName = url.getPath();
        this.reader   = new LineNumberReader(
                        new BufferedReader(
                        new InputStreamReader(
                        url.openStream())));
    }


    /**
     * Closes the FileWordReader.
     */
    public void close() throws IOException
    {
        if (reader != null)
        {
            reader.close();
        }
    }


    // Implementations for WordReader.

    protected String nextLine() throws IOException
    {
        return reader.readLine();
    }


    protected String lineLocationDescription()
    {
        return "line " + reader.getLineNumber() + " of file '" + fileName + "'";
    }
}
