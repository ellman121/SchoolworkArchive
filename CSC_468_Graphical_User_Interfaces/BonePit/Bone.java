/*

	**** Bone.java ****

This file is a data model for a bone.  It will keep track of wether or not to
draw itself, as well as all polyline data to do so.

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
 * The bone class holds all metadata and drawing information for a bone object
 *
 * @author Elliott Rarden
 */
public class Bone
{
	// Bone info
	// I'm leaving these public because using getters and setters
	// on private variables incurs three stack pushes each, which
	// isn't worth the performance dip to me
	public String id;
	public int year;
	public String taxon;
	public int objectNum;
	public int detail;
	public String completeness;
	public String gender;
	public String remarks;
	public double elevation;

	// Drawing info
	public Double xMin, xMax, yMin, yMax;
	public ArrayList<ArrayList<Point2D.Double>> polylines = new ArrayList<ArrayList<Point2D.Double>>();
	public Boolean isVisible;

	/**
	 * This is the constructor for the bone class.  This constructor creates a bone object with the given
	 * metadata, and then uses the ID to parse the xml file with that ID.
	 * 
	 * @param id 			The BoneID.  This will be used to load the bone data from the XML file
	 * @param year 			The year the bone was found
	 * @param taxon 		The taxonomy of the bone
	 * @param objectNum 	The object number of the bone
	 * @param detail		The "level of detail" of a bone
	 * @param completeness 	The bone's completeness grade
	 * @param gender 		The gender of the bone
	 * @param remarks 		Any remarks from the archeologists
	 * @param elevation 	The elevation of the bone at the site
	 * @return A new bone object reference
	 */
	public Bone(String id, int year, String taxon, int objectNum, int detail, String completeness, String gender, String remarks, double elevation) {

		// Put in all the stuff given to us
		this.id = id;
		this.year = year;
		this.taxon = taxon;
		this.objectNum = objectNum;
		this.detail = detail;
		this.completeness = completeness;
		this.gender = gender;
		this.remarks = remarks;
		this.elevation = elevation;

		// Parse the bone-specific xml file
		this.parseXML();
	}

	/**
	 * @return A string with debug information about the bone
	 */
	public String toString() {
		return "Bone Object " + id + "\n         year = " + year + "\n        taxon = " + taxon + "\n    objectNum = " + objectNum + "\n completeness = " + completeness + "\n      remarks = " + remarks + "\n    elevation = " + elevation + "\n   xMin, yMin = (" + xMin + ", " + yMin + ")\n   xMax, yMax = (" + xMax + ", " + yMax + ")\n";

	}

	/**
	 * This function finds, opens, and parses the XML file for the bone.  It
	 * then extracts all polyline information regarding the bone.  Thanks to
	 * Dr. Weiss for the sample XML reading files!
	 */
	void parseXML() {
		// Get our root node
		Element root = null;
		try {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document document = builder.parse( Kiosk.path + id + ".xml" );
			root = document.getDocumentElement();
		} catch (Exception e) {
			// System.out.println( e.toString() );
			// This is the exception that occurs when the XML file 
			// doesn't exist
		}

		// If there was an error reading the document, root gets
		// set to null.  We just retun an empty object
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
		for (int i = 0; i < polys.getLength(); i++) {
			Node current = polys.item(i);
			ArrayList<Point2D.Double> temp = new ArrayList<Point2D.Double>();
			NodeList xyPairs = current.getChildNodes();

			for (int j = 0; j < xyPairs.getLength(); j++) {
				Node pair = xyPairs.item(j);

				if(pair.getNodeName().equals("xy")) {
					Scanner sc = new Scanner( pair.getTextContent().trim() );
					double x = Double.parseDouble(sc.next());
					double y = Double.parseDouble(sc.next());
					Point2D.Double p = new Point2D.Double(x, y);
					temp.add(p);
				}
			}
			this.polylines.add(temp);
		}
	} // End parseXML

}
