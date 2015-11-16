/**
 * Shape.java
 *
 * Abstract class at the top of the class hierarchy.
 * 
 * contains abstract methods for draw, move, getMidpointX, getMidpointY, and 
 * toString
 */
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;

public abstract class Shape
{
	public abstract void draw( Graphics g );
	public abstract void move( int dx, int dy );
	public abstract int getMidpointX();
	public abstract int getMidpointY();
	public abstract String toString();
}

