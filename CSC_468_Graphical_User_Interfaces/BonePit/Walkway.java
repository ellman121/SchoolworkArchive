/*

	**** Walkway.java ****

This file will read the walkway file and return a walkway ojbect 
so that the team can draw as necessary.  It is a singleton class.

Author: Elliott Rarden
Class:  CSC468 GUI Programming
Date:   Spring 2017
*/

import org.w3c.dom.*;
import javax.xml.parsers.*;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Point2D;
import javax.swing.*;

/**
 * The walkway is a singleton class which holds drawing information about the
 * walkway at the Mammoth Site.
 *
 * @author Elliott Rarden
 */
public class Walkway
{
	// Drawing information
	public double xMin, xMax, yMin, yMax;
	public ArrayList<ArrayList<Point2D.Double>> polylines = new ArrayList<ArrayList<Point2D.Double>>();

	// Singleton classes keep a private reference to themselves
	private static Walkway ww;

	/**
	 * The walkway is a singelton object, so it's constructor is private
	 * 
	 * @return A walkway object
	 */
	private Walkway()
	{
		super();

		this.parseXML();
	}
	
	/**
	 * @return A string containing debug output about the walkway
	 */
	public String toString() {
		return "Walkway\n    xMin, xMax = (" + xMin + ", " + xMax + ")\n    yMin, yMax = (" + yMin + ", " + yMax + ")\n";

	}

	/**
	 * This function is essentially a copy of the parseXML() function from the
	 * bone class
	 *
	 * @see  bone.java
	 */
	private void parseXML()
	{
		// Get our root node
		Element root = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document document = builder.parse( Kiosk.path + "walkway.xml" );
			root = document.getDocumentElement();
		} catch (Exception e) {
			// System.out.println( e.toString() );
		}

		if (root == null) {
			return;
		}

		// Get NodeLists because they're easy to handle
		// Man Java has a massive encapsulation fetish
		NodeList mins = root.getElementsByTagName("xymin");
		NodeList maxs = root.getElementsByTagName("xymax");
		NodeList polys = root.getElementsByTagName("polyline");

		// Get read out our mins and maxes  
		if ( mins.getLength() > 0 ) {
			Scanner sc = new Scanner( mins.item(0).getTextContent().trim() );
			xMin = sc.nextDouble();
			yMin = sc.nextDouble();
		}
		if ( maxs.getLength() > 0 ) {
			Scanner sc = new Scanner( maxs.item(0).getTextContent().trim() );
			xMax = sc.nextDouble();
			yMax = sc.nextDouble();
		}

		// Read out our polylines
		// Let me just say that it upsets me that I can't do a 
		// for-in loop over NodeLists.  It seems like a massive
		// oversight by Java's designers.  Like, who decided
		// that a NodeList shouldn't just be a List<Node> or an
		// ArrayList<Node>? I get that it may not be as performant,
		// but if you are also the group that writes the compiler,
		// can't you fix that by recognizing it in the parse tree?
		for (int i = 0; i < polys.getLength(); i++) {
			Node current = polys.item(i);
			ArrayList<Point2D.Double> temp = new ArrayList<Point2D.Double>();
			NodeList xyPairs = current.getChildNodes();
			
			for (int j = 0; j < xyPairs.getLength(); j++) {
				Node pair = xyPairs.item(j);
				
				if(pair.getNodeName().equals("xy")) {
					Scanner sc = new Scanner( pair.getTextContent().trim() );
					double x = sc.nextDouble();
					double y = sc.nextDouble();
					Point2D.Double p = new Point2D.Double(x, y);
					temp.add(p);
				}
			}
			this.polylines.add(temp);
		}
	}

	/**
	 * This method allows peopel to access the walkway 
	 * 
	 * @return A reference to the walkway singleton
	 */
	public static Walkway getWalkwayInstance()
	{
		// If the does not exist
		if (ww == null)
		{
			// Make a new walkway
			ww = new Walkway();	
		}

		// Singletons!
		return ww;
	}

}
