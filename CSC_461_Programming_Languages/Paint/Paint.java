/**
 * Paint.java
 * 
 * Simple paint program written in Java.  
 * 
 * Features:
 *  - Menu bar featuring 
 *
 * @author Elliott Rarden, Derek Lane
 * @version Fall 2015
 * @note Borrows some code written by John Weiss, Ph.D.
 */

 // Import these
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.File;
import java.util.*;

/** MenuDemo class
 *  This class just contains the main() function to create a MenuFrame (JFrame) window.
 */
public class Paint
{
	public static void main( String args[] )
	{
		MenuFrame paintWindow = new MenuFrame();
		paintWindow.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
		paintWindow.setVisible( true );
	}
}

/** MenuFrame class
 * This class does most of the heavy lifting: sets up window with drop-down menus and drawing area.
 */
class MenuFrame extends JFrame implements KeyListener
{
	// private data //
	private final String [] shapeMenuItems = { "Line", 
		"Empty Rectangle", "Empty Ellipse", "Filled Rectangle", 
		"Filled Ellipse" };
	private final String [] colorMenuItems = { "White", "Red", "Yellow",
		"Blue", "Orange", "Pink", "Cyan", "Magenta", "Black",
		"Gray", "Light Gray", "Dark Gray" };
						
	private JRadioButtonMenuItem [] mItems;
	private DrawPanel dpanel = new DrawPanel();

	/** MenuFrame constructor
	 *  This constructor builds the GUI.
	 */
	public MenuFrame()
	{
		// Set window title with superclass's constructor
		super( "Paint by Elliott and Derek" );
		addKeyListener( this );

		// Add content pane
		Container container = getContentPane();
		dpanel = new DrawPanel();
		container.add( dpanel );

		//
		// Create a menu bar
		//
		JMenuBar menuBar = new JMenuBar();
		setJMenuBar( menuBar );

		// File Menu
		JMenu menu = new JMenu( "File" );
		menuBar.add( menu );
		// File Menu Items
		JMenuItem mItem = new JMenuItem("Clear Screen");
		mItem.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent ae )
			{
				dpanel.reset();
			}
		} );
		menu.add( mItem );
				
		mItem = new JMenuItem( "Quit" );
		mItem.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent ae )
			{
				System.exit( 0 );
			}
		} );
		menu.add( mItem );

		mItem = new JMenuItem( "Quit" );
		mItem.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent ae )
			{
				System.exit( 0 );
			}
		} );
		menu.add( mItem );

		///////////////////////////////////////////////////////////////

		// Shapes Radio Menu -- Borrowed from Dr. Weiss
		menu = new JMenu( "Shape Select" );
		menuBar.add( menu );
		ButtonGroup group = new ButtonGroup();
		mItems = new JRadioButtonMenuItem [ shapeMenuItems.length ];
		for ( int i = 0; i < shapeMenuItems.length; i++ )
		{
			mItems[i] = new JRadioButtonMenuItem( shapeMenuItems[i] );
			group.add( mItems[i] );
			mItems[i].addActionListener( new ActionListener()
			{
				public void actionPerformed( ActionEvent ae )
				{
					shapeMenuSelect( ae.getActionCommand() );
				}
			} );
			menu.add( mItems[i] );
		}
		mItems[0].setSelected( true );

		///////////////////////////////////////////////////////////////

		// shape Color Selection Radio Menu
		menu = new JMenu( "Shape Trim Color" );
		menuBar.add( menu );
		group = new ButtonGroup();
		mItems = new JRadioButtonMenuItem[ colorMenuItems.length ];
		for( int i = 0; i < colorMenuItems.length; i++ )
		{
			mItems[i] = new JRadioButtonMenuItem( colorMenuItems[i] );
			group.add( mItems[i] );
			mItems[i].addActionListener( new ActionListener()
			{
			public void actionPerformed( ActionEvent ae )
			{
				selectShapeColor( ae.getActionCommand() );
			}
			});
			menu.add( mItems[i] );
		}
		mItems[1].setSelected( true );

		///////////////////////////////////////////////////////////////

		// Background Color Selection Radio Menu
		menu = new JMenu( "Shape Background Color" );
		menuBar.add( menu );
		group = new ButtonGroup();
		mItems = new JRadioButtonMenuItem[ colorMenuItems.length ];
		for( int i = 0; i < colorMenuItems.length; i++ )
		{
			mItems[i] = new JRadioButtonMenuItem( colorMenuItems[i] );
			group.add( mItems[i] );
			mItems[i].addActionListener( new ActionListener()
			{
			public void actionPerformed( ActionEvent ae )
			{
				selectfillColor( ae.getActionCommand() );
			}
			});
			menu.add( mItems[i] );
		}
		mItems[0].setSelected( true );

		///////////////////////////////////////////////////////////////

		// Canvas Color Selection Radio Menu
		menu = new JMenu( "Canvas Color" );
		menuBar.add( menu );
		group = new ButtonGroup();
		mItems = new JRadioButtonMenuItem[ colorMenuItems.length ];
		for( int i = 0; i < colorMenuItems.length; i++ )
		{
			mItems[i] = new JRadioButtonMenuItem( colorMenuItems[i] );
			group.add( mItems[i] );
			mItems[i].addActionListener( new ActionListener()
			{
			public void actionPerformed( ActionEvent ae )
			{
				selectCanvasColor( ae.getActionCommand() );
			}
			});
			menu.add( mItems[i] );
		}
		mItems[0].setSelected( true );

		///////////////////////////////////////////////////////////////

		// Help menu -- Borrowed from Dr. Weiss
		menu = new JMenu( "Help" );
		menuBar.add( menu );
		mItem = new JMenuItem( "Help" );
		mItem.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent ae )
			{
				JOptionPane.showMessageDialog( null, "Press d to undo\nPress c to clear the screen\nPress q to quit", "About Paint", JOptionPane.INFORMATION_MESSAGE );
			}
		} );
		menu.add( mItem );
		mItem = new JMenuItem( "About" );
		mItem.addActionListener( new ActionListener()
		{
			public void actionPerformed( ActionEvent ae )
			{
				JOptionPane.showMessageDialog( null, "JavaPaint by Derek and Elliott", "About Paint", JOptionPane.INFORMATION_MESSAGE );
			}
		} );
		menu.add( mItem );

		// set initial window size
		setSize( 640, 480 );

		// Reset everything before it become visible
		dpanel.reset();

		// display window
		setVisible( true );
	}

	// Shape Menu Select
	private boolean shapeMenuSelect( String s )
	{
		for ( int i = 0; i < shapeMenuItems.length; i++ )
		{
			if ( s == shapeMenuItems[i] )
			{
				for ( int j = 0; j < shapeMenuItems.length; j++ )
					mItems[j].setSelected( j == i );

				// Set our shape in the draw panel
				dpanel.setShape(s);
				return true;
			}
		}
		return false;
	}

	// Color Menu Select
	private boolean selectShapeColor( String s )
	{
		for ( int i = 0; i < colorMenuItems.length; i++ )
		{
			if ( s == colorMenuItems[i] )
			{
				for ( int j = 0; j < colorMenuItems.length; j++ )
					mItems[j].setSelected( j == i );

				// Set our shape in the draw panel
				dpanel.setShapeColor(s);
				return true;
			}
		}
		return false;
	}

	// Set the background color
	private boolean selectfillColor( String s )
	{
		for ( int i = 0; i < colorMenuItems.length; i++ )
		{
			if ( s == colorMenuItems[i] )
			{
				for ( int j = 0; j < colorMenuItems.length; j++ )
					mItems[j].setSelected( j == i );

				// Set our shape in the draw panel
				dpanel.setfillColor(s);
				return true;
			}
		}
		return false;
	}

	// Set the Canvas color
	private boolean selectCanvasColor( String s )
	{
		for ( int i = 0; i < colorMenuItems.length; i++ )
		{
			if ( s == colorMenuItems[i] )
			{
				for ( int j = 0; j < colorMenuItems.length; j++ )
					mItems[j].setSelected( j == i );

				// Set our shape in the draw panel
				dpanel.setCanvasColor(s);
				return true;
			}
		}
		return false;
	}

	// KeyListener methods
	public void keyReleased( KeyEvent event ) { }
	public void keyTyped( KeyEvent event ) { }

	public void keyPressed( KeyEvent event )
	{
		// Escape key or 'Q' or 'q'
		if ( event.getKeyCode() == 27 || event.getKeyCode() == 81 || event.getKeyCode() == 113)
			System.exit( 0 );

		// 'd' and 'D' delete the top object
		if ( event.getKeyCode() == 68 || event.getKeyCode() == 100)
		{
			dpanel.popShape();
			repaint();
		}

		// 'c' and 'C' clear the screen
		if ( event.getKeyCode() == 67 || event.getKeyCode() == 99 ) {
			dpanel.reset();
		}
	}
}

/** DrawPanel class
 *  This class configures the drawing panel to support mouse events.
 */
class DrawPanel extends JPanel implements MouseListener
{
	// Variables to handle input and output points from mouse
	private int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
	private boolean leftButtonPress = false;
	private boolean rightButtonPress = false;
	private boolean drawNewShape = false;

	// Set variables to keep track of current shape and color
	private String currentShape = "Line";
	private Color trimColor = Color.red; // Trim color
	private Color fillColor = Color.red; // Background color

	// Shape stack
	ArrayList<Shape> shapeStack = new ArrayList<Shape>();

	// Canvas color
	private Color canvasColor = Color.white;

	// constructor: set up window
	public DrawPanel()
	{
		// Add a mouse click listener
		addMouseListener( this );
		shapeStack.clear();
		this.reset();
	}

	private double euclidianDistance(int x1, int y1, int x2, int y2)
	{
		return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
	}

	public void popShape()
	{
		if (shapeStack.size() > 0) {
			shapeStack.remove(shapeStack.size() - 1);	
		}
	}

	// We don't use these methods, but we need to them
	public void mouseClicked( MouseEvent e ) { }
	public void mouseEntered( MouseEvent e ) { }
	public void mouseExited( MouseEvent e ) { }

	// mouse button press events (start drawing)
	public void mousePressed( MouseEvent clickDown )
	{
		// check for left mouse button press
		if ( SwingUtilities.isLeftMouseButton(  clickDown ) )
		{
			x1 = clickDown.getX();
			y1 = clickDown.getY();
			leftButtonPress = true;
			return;
		}
		if ( SwingUtilities.isRightMouseButton( clickDown ) )
		{
			x1 = clickDown.getX();
			y1 = clickDown.getY();
			rightButtonPress = true;
			return;
		}
	}

	// mouse button release events (finish drawing)
	public void mouseReleased( MouseEvent event )
	{
		if ( leftButtonPress )
		{
			x2 = event.getX();
			y2 = event.getY();
			leftButtonPress = false;
			drawNewShape = true;
			repaint();
		}
		if ( rightButtonPress )
		{
			x2 = event.getX();
			y2 = event.getY();
			rightButtonPress = false;
			moveClosestShape();
			repaint();
		}
	}

	//Function for right-click moving shapes
	private void moveClosestShape()
	{
		if (shapeStack.size() > 0) {
			// Find the closest shape to x1, y1
			int shapeToMoveIndex = 0;
			double dist = euclidianDistance(x1, y1, shapeStack.get(0).getMidpointX(), shapeStack.get(0).getMidpointY());
			
			// Find the closest shape
			for (int i = 0; i < shapeStack.size(); i++)
			{
				double newdist = euclidianDistance(x1, y1, shapeStack.get(i).getMidpointX(), shapeStack.get(i).getMidpointY());
				if (newdist < dist)
				{
					dist = newdist;
					shapeToMoveIndex = i;
				}	
			}

			// If the distance is beyond a threshhold, ignore the command
			if (dist > 70) {
				return;
			}

			// Move the shape and repaint
			shapeStack.get(shapeToMoveIndex).move(x2 - x1, y2 - y1);
			repaint();
		}
	}

	// Reset all local variables and clear the stack
	public void reset()
	{
		shapeStack.clear();
		x1 = -1;
		y1 = -1;
		x2 = -1;
		y2 = -1;
		canvasColor = Color.white;
		leftButtonPress = false;
		drawNewShape = false;
		repaint();
	}

	public void setShape( String shapeName )
	{
		// Set the current shape
		currentShape = shapeName;
	}

	// Set current color based on the name passed in
	public void setShapeColor( String colorName )
	{
		switch (colorName) {
			case "Red":
				trimColor = Color.red;
				break;
			case "Yellow":
				trimColor = Color.yellow;
				break;
			case "Blue":
				trimColor = Color.blue;
				break;
			case "Orange":
				trimColor = Color.orange;
				break;
			case "Pink":
				trimColor = Color.pink;
				break;
			case "Cyan":
				trimColor = Color.cyan;
				break;
			case "Magenta":
				trimColor = Color.magenta;
				break;
			case "Black":
				trimColor = Color.black;
				break;
			case "White":
				trimColor = Color.white;
				break;
			case "Gray":
				trimColor = Color.gray;
				break; 
			case "Light Gray":
				trimColor = Color.lightGray;
				break;
			case "Dark Gray":
				trimColor = Color.darkGray;
				break;
		}
	}

	// Set current color based on the name passed in
	public void setfillColor( String colorName )
	{
		switch (colorName) {
			case "Red":
				fillColor = Color.red;
				break;
			case "Yellow":
				fillColor = Color.yellow;
				break;
			case "Blue":
				fillColor = Color.blue;
				break;
			case "Orange":
				fillColor = Color.orange;
				break;
			case "Pink":
				fillColor = Color.pink;
				break;
			case "Cyan":
				fillColor = Color.cyan;
				break;
			case "Magenta":
				fillColor = Color.magenta;
				break;
			case "Black":
				fillColor = Color.black;
				break;
			case "White":
				fillColor = Color.white;
				break;
			case "Gray":
				fillColor = Color.gray;
				break; 
			case "Light Gray":
				fillColor = Color.lightGray;
				break;
			case "Dark Gray":
				fillColor = Color.darkGray;
				break;
		}
		repaint();
	}

		// Set current color based on the name passed in
	public void setCanvasColor( String colorName )
	{
		switch (colorName) {
			case "Red":
				canvasColor = Color.red;
				break;
			case "Yellow":
				canvasColor = Color.yellow;
				break;
			case "Blue":
				canvasColor = Color.blue;
				break;
			case "Orange":
				canvasColor = Color.orange;
				break;
			case "Pink":
				canvasColor = Color.pink;
				break;
			case "Cyan":
				canvasColor = Color.cyan;
				break;
			case "Magenta":
				canvasColor = Color.magenta;
				break;
			case "Black":
				canvasColor = Color.black;
				break;
			case "White":
				canvasColor = Color.white;
				break;
			case "Gray":
				canvasColor = Color.gray;
				break; 
			case "Light Gray":
				canvasColor = Color.lightGray;
				break;
			case "Dark Gray":
				canvasColor = Color.darkGray;
				break;
		}
		repaint();
	}

	// paintComponent() is the repaint callback function
	public void paintComponent( Graphics g )
	{
		super.paintComponent( g );			// call superclass's paint method

		g.setColor(canvasColor);
		g.fillRect(-1, -1, this.getSize().width + 2, this.getSize().height + 2);

		for (Shape s : shapeStack)
		{
			s.draw(g);
		}

		// Make sure x1 and y1 are the top left
		if(x1 > x2 && currentShape != "Line")
		{
			int temp = x2;
			x2 = x1;
			x1 = temp;
		}
		if(y1 > y2 && currentShape != "Line")
		{
			int temp = y2;
			y2= y1;
			y1 = temp;
		}
		
		if (drawNewShape)
		{
			System.out.println("Drawing a " + currentShape + " with trimColor " + trimColor);
			switch (currentShape) {
			case "Line":
				Line l = new Line(x1, y1, x2, y2, trimColor);
				l.draw(g);
				shapeStack.add(l);
				break;

			case "Empty Rectangle":
				Rectangle r = new Rectangle(x1, y1, x2, y2, trimColor);
				r.draw(g);
				shapeStack.add(r);
				break;

			case "Empty Ellipse":
				Ellipse e = new Ellipse(x1, y1, x2, y2, trimColor);
				e.draw(g);
				shapeStack.add(e);
				break;

			case "Filled Rectangle":
				FilledRectangle fr = new FilledRectangle(x1, y1, x2, y2, fillColor, trimColor);
				fr.draw(g);
				shapeStack.add(fr);
				break;

			case "Filled Ellipse":
				FilledEllipse fe = new FilledEllipse(x1, y1, x2, y2, fillColor, trimColor);
				fe.draw(g);
				shapeStack.add(fe);
				break;

			default:
				break;
			}	
		}
		drawNewShape = false;
	}
}
