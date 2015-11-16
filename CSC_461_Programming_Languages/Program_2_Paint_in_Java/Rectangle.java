/**
 * Rectangle.java
 */
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

public class Rectangle extends Shape
{
	private int topLeftX;
	private int topLeftY;
	private int width;
	private int height;
	private Color shapeColor;
	
	//constructors
	Rectangle() {}
	Rectangle( int x1, int y1, int x2, int y2, Color col)
	{
		// Find topmost left x and y and set to topLeftX/Y respectivly
		topLeftX = Math.min(x1, x2); // Minimum value is leftmost
		topLeftY = Math.min(y1, y2); // Minimum value is top

		width = Math.abs(x1 - x2); // Width is absolute value of difference of x coordinates
		height = Math.abs(y1 - y2); // height is absolute value of difference of x coordinates

		shapeColor = col;
	}

		//draw from Shape
	public void draw( Graphics g )
	{
		g.setColor( shapeColor );
		g.drawRect( topLeftX, topLeftY, width, height );
	}
	
		//getMidPointX() from shape
	public int getMidpointX()
	{
		return (width/2) + topLeftX;
	}

		//getMidpointY() from Shape
	public int getMidpointY()
	{
		return (height/2) + topLeftY;
	}

		//move from Shape
	public void move( int dx, int dy )
	{
		topLeftX += dx;
		topLeftY += dy; 
	}
		
	//toString from shape
	public String toString()
	{
		String shapeOutput = "Rectangle, Color: " + shapeColor;
		
		return shapeOutput;
	}
}
