/* $Id: TextAreaOutputStream.java,v 1.5 2004/08/15 12:39:30 eric Exp $
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
package proguard.gui;

import java.io.*;

import javax.swing.*;


/**
 * This <code>PrintStream</code> appends its output to a given text area.
 *
 * @author Eric Lafortune
 */
class TextAreaOutputStream extends FilterOutputStream implements Runnable
{
    private JTextArea textArea;


    public TextAreaOutputStream(JTextArea textArea)
    {
        super(new ByteArrayOutputStream());

        this.textArea = textArea;
    }


    // Implementation for FilterOutputStream.

    public void flush() throws IOException
    {
        super.flush();

        // Append the accumulated buffer contents to the text area.
        SwingUtil.invokeAndWait(this);
    }


    // Implementation for Runnable.

    public synchronized void run()
    {
        ByteArrayOutputStream out = (ByteArrayOutputStream)super.out;

        // Has any new text been written?
        String text = out.toString();
        if (text.length() > 0)
        {
            // Append the accumulated text to the text area.
            textArea.append(text);

            // Clear the buffer.
            out.reset();
        }
    }
}
