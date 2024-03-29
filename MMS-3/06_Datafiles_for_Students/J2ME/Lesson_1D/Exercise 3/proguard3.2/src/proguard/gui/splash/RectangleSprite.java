/* $Id: RectangleSprite.java,v 1.6 2004/08/15 12:39:30 eric Exp $
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
package proguard.gui.splash;

import java.awt.Graphics;

/**
 * This Sprite represents an animated rounded rectangle. It can optionally be filled.
 *
 * @author Eric Lafortune
 */
public class RectangleSprite implements Sprite
{
    private boolean       filled;
    private VariableColor color;
    private VariableInt   x;
    private VariableInt   y;
    private VariableInt   width;
    private VariableInt   height;
    private VariableInt   arcWidth;
    private VariableInt   arcHeight;


    /**
     * Creates a new rectangular RectangleSprite.
     * @param filled specifies whether the rectangle should be filled.
     * @param color  the variable color of the rectangle.
     * @param x      the variable x-coordinate of the upper-left corner of the rectangle.
     * @param y      the variable y-coordinate of the upper-left corner of the rectangle.
     * @param width  the variable width of the rectangle.
     * @param height the variable height of the rectangle.
     */
    public RectangleSprite(boolean       filled,
                           VariableColor color,
                           VariableInt   x,
                           VariableInt   y,
                           VariableInt   width,
                           VariableInt   height)
    {
        this(filled, color, x, y, width, height, new ConstantInt(0), new ConstantInt(0));
    }


    /**
     * Creates a new RectangleSprite with rounded corners.
     * @param filled specifies whether the rectangle should be filled.
     * @param color      the variable color of the rectangle.
     * @param x          the variable x-coordinate of the upper-left corner of the rectangle.
     * @param y          the variable y-coordinate of the upper-left corner of the rectangle.
     * @param width      the variable width of the rectangle.
     * @param height     the variable height of the rectangle.
     * @param arcwWidth  the variable width of the corner arcs.
     * @param arcwHeight the variable height of the corner arcs.
     */
    public RectangleSprite(boolean       filled,
                           VariableColor color,
                           VariableInt   x,
                           VariableInt   y,
                           VariableInt   width,
                           VariableInt   height,
                           VariableInt   arcWidth,
                           VariableInt   arcHeight)
    {
        this.filled    = filled;
        this.color     = color;
        this.x         = x;
        this.y         = y;
        this.width     = width;
        this.height    = height;
        this.arcWidth  = arcWidth;
        this.arcHeight = arcHeight;
    }

    // Implementation for Sprite.

    public void paint(Graphics graphics, long time)
    {
        graphics.setColor(color.getColor(time));

        int xt = x.getInt(time);
        int yt = y.getInt(time);
        int w  = width.getInt(time);
        int h  = height.getInt(time);
        int aw = arcWidth.getInt(time);
        int ah = arcHeight.getInt(time);

        if (filled)
        {
            graphics.fillRoundRect(xt, yt, w, h, aw, ah);
        }
        else
        {
            graphics.drawRoundRect(xt, yt, w, h, aw, ah);
        }
    }
}
