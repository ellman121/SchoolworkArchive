/*

	**** BonePit.java ****

References: This file is a adapted from Dr. John Weiss' XMLplot2.java code

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
 * The BonePit class is a "static class" in that it only has static methods.  
 * It class will parse the XML bones.xml file and create a list of BoneObjects,
 * as well as hold any metadata about the bonepit at the mammoth site, such as
 * the maximum and minimum elevation of a bone.
 *
 * @author Elliott Rarden
 */
public class BonePit {
	private static ArrayList<Bone> bones = null;
	private static double minElevation;
	private static double maxElevation;

	/**
	 * @return The minimum elevation in the bone set
	 */
	public static double getMinElevation( ) {
		if (bones == null) {
			readBones();
		}

		return minElevation;
	}

	/**
	 * @return The maximum elevation in the bone set
	 */
	public static double getMaxElevation( ) {
		if (bones == null) {
			readBones();
		}

		return maxElevation;
	}

	/**
	 * @brief This is a static method to have the BonePit class read the XML files and return them as an ArrayList of Bone objects.
	 * 
	 * @return An ArrayList of Bone objects
	 */
	public static ArrayList<Bone> readBones( )
	{
		// PseudoSingleton
		// If bones exists, just return it
		if (bones != null) {
			return bones;
		}

		// Variables
		bones = new ArrayList<>();
		NodeList bonerecs = null;

		// Walk through the XML file, if it's a bonerec then we will add its
		// data to a new bone and push it onto our ArrayList.
		try {
			// Get root node, generate list of bonerec objects
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document document = builder.parse( Kiosk.path + "bones.xml" );
			document.normalize();
			Element root = document.getDocumentElement();
			bonerecs = root.getChildNodes();
		} catch (Exception e) {
			System.out.println("Error opening bones.xml");
		}

		if (bonerecs == null) {
			System.out.println("bonerecs is null");
			return null;
		}

		// For each bonerec object, we build a bone object and add it to our list
		for (int i = 1; i < bonerecs.getLength(); i++) {
			Node bonerec = bonerecs.item(i);
			NodeList children = bonerec.getChildNodes();
			String id = "", taxon = "", completeness = "", remarks = "", gender = "UNDESIGNATED";
			int year = 0, objectNum = 0, element = 0;
			double elevation = 0.0;

			// For some reason, it reads "null" nodes in between each actual
			// node.  This ensures we don't crash
			if (!bonerec.getNodeName().equals("bonerec")) {
				continue;
			}

			// Read out the data we care about from the node
			for (int j = 0; j < children.getLength(); j++) {
				Node child = children.item(j);

				// System.out.println("Child is " + child);

				if (child.getNodeName().equals("uniqueid")) {
					id = child.getTextContent().trim();
				}
				else if (child.getNodeName().equals("year")) {
					Scanner sc = new Scanner( child.getTextContent().trim() );
					year = Integer.parseInt(sc.next());
				}
				else if (child.getNodeName().equals("taxon")) {
					taxon = child.getTextContent().trim();
				}
				else if (child.getNodeName().equals("objectnum")) {
					Scanner sc = new Scanner( child.getTextContent().trim() );
					objectNum = Integer.parseInt(sc.next());
				}
				else if (child.getNodeName().equals("completeness")) {
					completeness = child.getTextContent().trim();
				}
				else if (child.getNodeName().equals("remarks")) {
					remarks = child.getTextContent().trim();
				}
				else if (child.getNodeName().equals("gender")) {
					gender = child.getTextContent().trim();
				}
				else if (child.getNodeName().equals("elevation")) {
					Scanner sc = new Scanner( child.getTextContent().trim() );
					elevation = Double.parseDouble(sc.next());
					if (elevation < minElevation) {
						minElevation = elevation;
					} else if (elevation > maxElevation) {
						maxElevation = elevation;
					}
				}
				else if (child.getNodeName().equals("element")) {
					Scanner sc = new Scanner( child.getTextContent().trim() );
					element = Integer.parseInt(sc.next());
				}
			}

			// Debug output
			// System.out.println("Creating bone with uniqueid " + id);

			// 1 Liner to wait for the user to press enter
			// Really unsafe, don't use in production code
			// try { System.in.read(); } catch (Exception e) { }

			// Build a Bone Object and add it to our array
			bones.add(new Bone(id, year, taxon, objectNum, element, completeness, gender, remarks, elevation));
		}

		// Return our list of bones
		return bones;
	}

	// main function for testing this class
	public static void main( String[] args ) {
		ArrayList<Bone> bones = BonePit.readBones();

		Set<Integer> details = new HashSet<>();
		for (Bone bone : bones) {
			details.add(bone.detail);
		}

		for (Integer s : details) {
			System.out.println(s);
		}
	}
}
