/**
 * Line.java
 */
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

public class Line extends Shape
{
	private int startX;
	private int startY;
	private int endX;
	private int endY;
	private Color c;

	//constructor
	public Line( int x1, int y1, int x2, int y2, Color col )
	{
		startX = x1;
		startY = y1;
		endX = x2;
		endY = y2;
		c = col;
	}

		//draw from Shape
	public void draw( Graphics g)
	{
		g.setColor( c );
		g.drawLine( startX, startY, endX, endY );
	}

		//getMidpointX from Shape
	public int getMidpointX()
	{
		return (startX + endX) / 2;
	}
	
		//getMidpointY from Shape
	public int getMidpointY()
	{
		return (startY + endY) / 2;
	}

		//move from Shape
	public void move( int dx, int dy )
	{
		startX += dx;
		startY += dy;
		endX += dx;
		endY += dy;
	}
		
	//toString from Shape
	public String toString()
	{
		String shapeOutput = "Line, Color: " + c;
		
		return shapeOutput;
	}
}
