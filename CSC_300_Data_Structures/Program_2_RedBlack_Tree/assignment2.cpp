/*************************************************************************//**
 * Assignment 2 - Red-Black Trees
 *
 * Elliott Rarden && Weston Silbaugh
 * 
 * October 2, 2014
 * Professor: Dr. Ed Corwin
 * Course: CSC 300 - 10:00am
 * Location: McLaurey 205
 *
 * Program Information
 * 	This program reads in integers from a text file, and arranges them into a
 * Red Black Tree in a binary file on disk.  It will then search through the 
 * Binary file for the smallest value, Root value, and Largest Value and 
 * output them, along with the height of the tree, number of nodes read,
 * and the number of nodes written
 *
 * Compiling Instructions: 
 *      Use g++ to compile, uses C++11 Functions
 *      Include rb_tree.h and rb_tree.cpp in the same directory as this file
 *****************************************************************************/
#include "rb_tree.h"

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
bool numberIsInTree(std::vector<int> v, int test);
int getSmallestValue(int height);
int getLargestValue(int nodesRead);

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * This is the main funciton for a probram which will read in 32-bit integers
 * from a text file and insert them into a red-black tree on a binary file on 
 * disk.  Note, this is a disk-intensive program and is not suggested for solid
 * state drives.  
 * 
 * Paramaters:
 *   IN  - Number of command-line arguments
 *       - 2D Array containing argument values
 * 
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
 int main(int argc, char **argv)
 {
 	int nodesRead = 0; // Keep track of the number of nodes we read in from the first file
 	int nodesWritten = 0; // Keep track of number of writes we make to the file
 	int height = 0; // We will calculate the height of the tree
 	int temp = 0; // Temporary variable for input

 	ifstream textin;

	vector<int> v;
	vector<int> values;  // Add each unique number read from the text file
 						 // into this vector to check for duplicates

	string out1 = "Number of nodes read:  ";
	string out2 = "Number of nodes written:  ";
	string out3 = "The height of this tree:  ";
	string out4 = "Root value:  ";
	string out5 = "Smallest value in leaf:  ";
	string out6 = "Largest value in leaf:  ";

	// Open input and output files
	textin.open("text.in");

	while(cin >> temp)
	{
		if(numberIsInTree(v, temp))
		{
			cerr << "Found duplicate value - Will skip" << endl;
			continue;
		}
		v.push_back(temp);
		insert(temp, nodesWritten);
		nodesRead++;
	}

	textin.close();

 	// Output to Standard Output
 	cout << out1 << nodesRead << endl;
 	cout << out2 << nodesWritten << endl;
 	cout << out3 << height << endl;
 	cout << out4 << read_root().key_value << endl;
 	cout << out5 << getSmallestValue(fin) << endl;
 	cout << out6 << getLargestValue(nodesRead) << endl;

 	return 0;
 }

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * Return true if the number is found
 * 
 * Paramaters:
 *   IN  - vector of integers to search through
 *       - The number to look for
 * 
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
bool numberIsInTree(std::vector<int> v, int test)
{
	for (int i = 0; i < v.size(); i++)
	{
		if(v[i] == test)
			return true;
	}
	return false;
}


/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * This funciton returns the smallest value in the tree
 * 
 * Paramaters:
 *   IN  height - The height of the tree
 * 
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
int getSmallestValue(int height)
{
	int value = 0;
	int pos = pow(2, height) - 1; // Smallest value is node # 2^h - 1
	return read_node(pos).key_value;
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * This function returns the largets value on the tree
 *
 * Paramaters:
 *   IN  nodesRead - The number of nodes in the tree
 *
 * Returns:
 *   Value of the last node on the file
==============================================================================*/
int getLargestValue(int nodesRead)
{
	return read_node(nodesRead - 1).key_value;
}
