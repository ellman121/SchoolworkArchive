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
 * output them, along with the hight of the tree, number of nodes read,
 * and the number of nodes written.  Sorry for the second submit, forgot to 
 * add documentation to main()
 *
 * Compiling Instructions: 
 *      Use g++ to compile, uses C++11 Functions
 *****************************************************************************/
#include <cmath>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

fstream file;

int nodes_written;

struct rbtree_node
  {
  int  key_value;
  int  left_child;    // child file address
                      // [record index, not byte address]
  int  right_child; 
  bool color;      // true if left child is red
  bool right_red;     // true if right child is red

  bool pad1;          // padding since compiler allocates two extra bytes
  bool pad2;          // padding since compiler allocates two extra bytes

  // may need filler to force zeros in any padding of the struct
  int  file_loc;      // location on file (record index)
  };
const int RB_NULL = -1;
const int RB_NODE_SIZE  = sizeof(rbtree_node);

enum color { BLACK, RED };

/******************************************************************************
 *                         Function Prototypes                                *
 ******************************************************************************/
/* ==== MISC FUNCTIONS === */
bool numberIsInTree(std::vector<int> v, int test);
int getSmallestValue(int &file_loc);
int getLargestValue(int &file_loc);

/* ==== FAMILY FUNCTIONS ==== */
rbtree_node grandparent(rbtree_node n);
rbtree_node left_child(rbtree_node n);
rbtree_node new_node(int file_loc = RB_NULL, int val = RB_NULL, bool color = RED,
	int left = RB_NULL, int right = RB_NULL);
rbtree_node parent(rbtree_node n);
rbtree_node right_child(rbtree_node n);
rbtree_node sibling(rbtree_node n);
rbtree_node uncle(rbtree_node n);

/* ==== INSERT FUNCTIONS ==== */
void insert(int val);
void insert_case1(rbtree_node &n);
void insert_case2(rbtree_node &n);
void insert_case3(rbtree_node &n);
void insert_case4(rbtree_node &n);
void insert_case5(rbtree_node &n);

/* ==== ROTATE FUNCTIONS ==== */
void rotate_left(rbtree_node &p);
void rotate_right(rbtree_node &p);
void rotate_tree(rbtree_node &g, rbtree_node &p, rbtree_node &n);
void swap_node(rbtree_node &dest, rbtree_node &source);

/* ==== FILE I/O FUNCTIONS ==== */
void readFile(ostream &out);
rbtree_node read_node(int file_loc);
rbtree_node read_root();
void write_node(rbtree_node &n);
void write_node(int file_loc, rbtree_node &n);
int convertTreeAndGetHeight(int file_loc);

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
int main( int argc, char **argv)
{
	ifstream textin;
	int nodesRead = 0;
 	int height = 0;
 	int temp = 0;
 	int small, large;
 	nodes_written = 0;

	vector<int> v;// Add each unique number read from the text file
 				  // into this vector to check for duplicates

	string out1 = "Number of nodes read:  ";
	string out2 = "Number of nodes written:  ";
	string out3 = "The hight of this tree:  ";
	string out4 = "Root value:  ";
	string out5 = "Smallest value in leaf:  ";
	string out6 = "Largest value in leaf:  ";

	textin.open(argv[1]);
	file.open(argv[2], ios::binary | ios::in | ios::out |ios::trunc);
	while(textin >> temp)
	{
		if(numberIsInTree(v, temp))
		{
			cerr << "Found duplicate value - Will skip" << endl;
			continue;
		}
		v.push_back(temp);
		insert(temp);
		nodesRead++;
	}

	height = convertTreeAndGetHeight(0);
	temp = 0;
	small = getSmallestValue(temp);
	temp = 0;
	large = getLargestValue(temp);

 	// Output to Standard Output
 	cout << out1 << nodesRead << endl;
 	cout << out2 << nodes_written << endl;
 	cout << out3 << height << endl;
 	cout << out4 << read_root().key_value << endl;
 	cout << out5 << small << endl;
 	cout << out6 << large << endl;

	textin.close();

 	return 0;
}

/* ==========================================================================
 * =                     FUNCTION IMPLEMENTATIONS                           =
 * ========================================================================== */

/* ============================ FAMILY FUNCTIONS ============================ */

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return the grandparent of a node
 * 
 * Paramaters:
 *   IN: n - Node in question
 * 
 * Returns:
 *   Grandparent node (Will return root if it doesn't exist)
==============================================================================*/
rbtree_node grandparent(rbtree_node n)
{
	rbtree_node r = read_root();
	rbtree_node p = parent(n);

	if (n.file_loc == r.file_loc) // The root node doens't have a grandparent
	{
		return new_node(RB_NULL); //Return null node
	}

	if (p.file_loc == r.file_loc) // The child of the root doesn't have a grandparent
	{
		return new_node(RB_NULL); //Return null node
	}

	return parent(p); // Return grandparent
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return the left child of a node
 * 
 * Paramaters:
 *   IN: n - Node in question
 * 
 * Returns:
 *   left_child *  (Will return a new node if there is no child)
==============================================================================*/
rbtree_node left_child(rbtree_node n)
{
	if (n.file_loc == RB_NULL || n.left_child == RB_NULL) // NULL case
	{
		return new_node(RB_NULL); //Return null node
	}
	return read_node(n.left_child);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return an empty node
 * 
 * Paramaters:
 *   IN:    file_loc - location of the new node on file
 * 			val - Node's value
 * 			color - Red or Black
 * 			left - File location of the new node's left child
 * 			right - File location of the new node's right child
 * 
 * Returns:
 * 		rbtree_node
==============================================================================*/
rbtree_node new_node(int file_loc, int val, bool color, int left, int right)
{
	rbtree_node n;
	n.file_loc = file_loc;
	n.key_value = val;
	n.color = color;
	n.right_red = false;
	n.pad1 = false;
	n.pad2 = false;
	n.left_child = left;
	n.right_child = right;
	return n;
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return the parent of a node
 * 
 * Paramaters:
 *   IN: n - Node in question
 * 
 * Returns:
 *   rbtree_node parent
==============================================================================*/
rbtree_node parent(rbtree_node n)
{
	rbtree_node parent;
	rbtree_node cur = read_root();

	if (n.file_loc == RB_NULL || cur.file_loc == RB_NULL) // NULL case
	{
		return new_node(RB_NULL); //Return null node
	}

	if(n.file_loc == cur.file_loc) // If n is the root node
		new_node(RB_NULL); //Return null node

	//search for n
	while (cur.file_loc != RB_NULL)
	{
		if (n.key_value == cur.key_value)
		{
			return parent;
		}
		parent = cur;
		if (n.key_value > cur.key_value)
		{
			cur = read_node(cur.right_child);
		}
		else
		{
			cur = read_node(cur.left_child);
		}
	}
	return cur;
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return the right child of a node
 * 
 * Paramaters:
 *   IN: n - Node in question
 * 
 * Returns:
 *   right_child (Will return a new node if there is no child)
==============================================================================*/
rbtree_node right_child(rbtree_node n)
{
	if (n.file_loc == RB_NULL || n.right_child == RB_NULL) // NULL Case
	{
		return new_node();
	}

	return read_node(n.right_child);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return the sibling of a node
 * 
 * Paramaters:
 *   IN: n - Node in question
 * 
 * Returns:
 *   sibling (Will return a new node if there is no child)
==============================================================================*/
rbtree_node sibling(rbtree_node n)
{
	if (n.file_loc == left_child(parent(n)).file_loc)  //n->parent->lef
		return right_child(parent(n)); //n->parent->right
	else
		return left_child(parent(n)); //n->parent->left
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Return the uncle of a node
 * 
 * Paramaters:
 *   IN: n - Node in question
 * 
 * Returns:
 *   uncle (Will return a new node if there is no child)
==============================================================================*/
rbtree_node uncle(rbtree_node n)
{
	rbtree_node g = grandparent(n);
	if (g.file_loc == RB_NULL)
		return new_node(RB_NULL); // No grandparent means no uncle
	if (parent(n).file_loc == g.left_child)
		return right_child(g);
	else
		return left_child(g);
}

/* ============================ INSERT FUNCTIONS ============================ */

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Insert a node into the tree
 * 
 * Paramaters:
 *   IN: val - The value to be inserted
==============================================================================*/
void insert(int val)
{
	rbtree_node n = new_node(RB_NULL, val);
	rbtree_node cur = read_root();
	rbtree_node pre = read_root();

	//top down traversal to find insert location
	while (cur.file_loc != RB_NULL)
	{
		pre = cur;
		if (val > cur.key_value)
		{
			cur = right_child(cur);
		}
		else
		{
			cur = left_child(cur);
		}
	}

	//insert into tree
	if (pre.file_loc == RB_NULL)
	{
		write_node(n);
	}
	else
	{
		if (val > pre.key_value) //insert at bottom of tree
		{
			write_node(n);
			pre.right_child = n.file_loc;
			write_node(pre.file_loc, pre);
		}
		else
		{
			write_node(n);
			pre.left_child = n.file_loc;
			write_node(pre.file_loc, pre);
		}
	}

	//check cases
	insert_case1(n);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Helper Function for insert(int val)
 * 
 * Paramaters:
 *   IN: n - Node for the value passed by reference
==============================================================================*/
void insert_case1(rbtree_node &n)
{
	if (n.file_loc == 0)
	{
		return;
	}
	else
		insert_case2(n);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Helper Function for insert(int val)
 * 
 * Paramaters:
 *   IN: n - Node for the value passed by reference
==============================================================================*/
void insert_case2(rbtree_node &n)
{
	rbtree_node p = parent(n);
	if (p.color == BLACK)
	{
		write_node(n.file_loc, n);
		return; /* Tree is still valid */
	}

	/* Since we return above, we don't need else - safer code this way */
	insert_case3(n);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Helper Function for insert(int val)
 * 
 * Paramaters:
 *   IN: n - Node for the value passed by reference
==============================================================================*/
void insert_case3(rbtree_node &n)
{
	rbtree_node u = uncle(n);
	rbtree_node p = parent(n);
	rbtree_node g;

	if ((u.file_loc != RB_NULL) && (u.color == RED))
	{
		p.color = BLACK;
		write_node(p.file_loc, p);
		u.color = BLACK;
		write_node(u.file_loc, u);
		g = grandparent(n);
		g.color = RED;
		write_node(g.file_loc, g);
		insert_case1(g);
	}
	if (p.color == RED)
		insert_case4(n);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Helper Function for insert(int val)
 * 
 * Paramaters:
 *   IN: n - Node for the value passed by reference
==============================================================================*/
void insert_case4(rbtree_node &n)
{
	rbtree_node g = grandparent(n);
	rbtree_node p = parent(n);

	if (n.file_loc == right_child(p).file_loc
		&& p.file_loc == left_child(g).file_loc)
	{
		rotate_left(p);

		//write_node(n.file_loc, left_child(n));//?
		n = p;
	}
	else if (n.file_loc == left_child(p).file_loc
		&& p.file_loc == right_child(g).file_loc)
	{
		rotate_right(p);
		//write_node(n.file_loc, right_child(n));//?
		n = p;
	}
		insert_case5(n);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Helper Function for insert(int val)
 * 
 * Paramaters:
 *   IN: n - Node for the value passed by reference
==============================================================================*/
void insert_case5(rbtree_node &n)
{
	rbtree_node g = grandparent(n);
	rbtree_node p = parent(n);

	p.color = BLACK;
	write_node(p.file_loc, p);
	g.color = RED;
	write_node(n.file_loc, n);
	if (n.file_loc == left_child(parent(n)).file_loc)
	{
		rotate_right(g); // Function terminates - safe code
		return;
	}
	rotate_left(g);
}

/* ============================ ROTATE FUNCTIONS ============================ */

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Perform a left rotate on disk
 * 
 * Paramaters:
 *   IN: a - Node at the top passed by reference
==============================================================================*/
void rotate_left(rbtree_node &a)
{
	rbtree_node b;
	rbtree_node y;

	if (a.file_loc == RB_NULL) // NULL Case
		return;

	b = right_child(a);

	if (b.file_loc != RB_NULL)
		y = left_child(b);

	//rebuild rotated tree
	swap_node(a, b);
	b.left_child = a.file_loc;
	a.right_child = y.file_loc;
	write_node(a.file_loc, a);
	write_node(b.file_loc, b);
	
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Perform a right rotate on disk
 * 
 * Paramaters:
 *   IN: b - Node at the top passed by reference
==============================================================================*/
void rotate_right(rbtree_node &b)
{
	rbtree_node a;
	rbtree_node y;

	if (b.file_loc == RB_NULL) // NULL Case
		return;

	a = left_child(b);

	if (a.file_loc != RB_NULL)
		y = right_child(a);

	//rebuild rotated tree
	swap_node(b, a);
	a.right_child = b.file_loc;
	b.left_child = y.file_loc;
	write_node(a.file_loc, a);
	write_node(b.file_loc, b);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Rotates the tree by performing the correct 
 * 
 * Paramaters:
 *   IN: the nodes in the triad to be rotated
==============================================================================*/
void rotate_tree(rbtree_node &a, rbtree_node &b, rbtree_node &c)
{
	
	if (a.key_value > b.key_value)
		swap(a, b);
	if (a.key_value > c.key_value)
		swap(a, c);
	if (b.key_value > c.key_value)
		swap(b,c);
	
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Swap two nodes on disk.  This destructively swaps the nodes, not values
 * 
 * Paramaters:
 *   IN: a - Node at the top passed by reference
==============================================================================*/
void swap_node(rbtree_node &dest, rbtree_node &source)
{
	int temp = source.file_loc;
	source.file_loc = dest.file_loc;
	dest.file_loc = temp;
}

/* =========================== FILE I/O FUNCTIONS =========================== */

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Sequentially read out every node's file_location, key_value, color, and children
 * 
 * Paramaters:
 *   IN: out - an output stream passed by reference
==============================================================================*/
void readFile( ostream &out)
{
	rbtree_node temp;
	int i,n;

	file.seekg(0, ios::end);
	n = ( file.tellg() / sizeof(rbtree_node) ) ;
	for (i = 0; i < n; i++)
	{
		temp = read_node(i);
		out << "  file_loc " << temp.file_loc << endl;
		out << "  key_value " << temp.key_value << endl;
		out << "  color " << temp.color << endl;
		out << "  left " << temp.left_child << endl;
		out << "  right " << temp.right_child << endl;
		out << endl;
	}
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Reads the root node in the file
 * 
 * Returns
 * 	The root node on the file
==============================================================================*/
rbtree_node read_root()
{
	file.seekg(0, ios::end);
	if (file.tellg()) // 0 if the file is empty
		return read_node(0);
	return new_node();
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Reads the root node in the file
 * 
 * Paramaters:
 *   IN: file_loc - the location of the requested node on disk
 *
 * Returns
 * 	The requested node
==============================================================================*/
rbtree_node read_node(int file_loc)
{
	int pos = 0;
	rbtree_node n;

	if (file_loc <= RB_NULL) // NULL Case
	{
		return new_node();
	}

	if (file_loc) // We know it's not negative from above.  0 will skip
		pos = file_loc*sizeof(rbtree_node);

	// Seek and read
	file.seekg(ios::beg + pos);
	file.read((char*)&n, sizeof(rbtree_node));
	return n;
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Write a node to disk
 * 
 * Paramaters:
 *   IN: n - The node to be written to disk passed by reference
==============================================================================*/
void write_node(rbtree_node &n)
{
	nodes_written++;
	file.seekg(0,ios::end);
	n.file_loc = file.tellg() / sizeof(rbtree_node);
	file.write((char*)&n, sizeof(rbtree_node));
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 	Write a node to disk
 * 
 * Paramaters:
 *   IN: 	file_loc - The destination location of the node on disk
 * 			n - The node to be written to disk passed by reference
==============================================================================*/
void write_node(int file_loc, rbtree_node &n)
{
	nodes_written++;
	if (file_loc == RB_NULL) // If null, insert at 0 (root)
		file_loc = 0;

	n.file_loc = file_loc;
	file.seekg(file_loc * sizeof(rbtree_node), ios::beg);
	file.write((char*)&n, sizeof(rbtree_node));
	file.seekg(ios::end);
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * 		Helper function for below
==============================================================================*/
int helper(rbtree_node cur)
{
	rbtree_node left_child = read_node(cur.left_child);
	rbtree_node right_child = read_node(cur.right_child);

	if(cur.file_loc == RB_NULL)
		return 0;

	if(left_child.file_loc != RB_NULL)
		cur.color = left_child.color;
	else
		cur.color = false;

	if(right_child.file_loc != RB_NULL)
		cur.right_red = right_child.color;
	
	write_node(cur.file_loc, cur);

	if(left_child.file_loc == RB_NULL && right_child.file_loc == RB_NULL)
		return 0;

	return 1;
}

/* =============================================================================
 * Author: Elliott Rarden & Weston Silbaugh
 *
 * Description:
 * Returns the hight of the tree, and converts tree format
 * 
 * Paramaters:
 *   IN  file_loc - location of node to convert on disk
 * 
 * Returns:
 *   Hight of the tree (from the first node)
==============================================================================*/
int convertTreeAndGetHeight( int file_loc)
{
	int height = 0;
	int left_h = 0;
	int right_h = 0;
	rbtree_node cur = read_node(file_loc);
	rbtree_node temp = new_node();

	height += helper(cur);

	if(cur.left_child != RB_NULL)
		left_h = convertTreeAndGetHeight( cur.left_child );
	if(cur.right_child != RB_NULL)
		right_h = convertTreeAndGetHeight( cur.right_child );


	return height + ( (left_h < right_h) ? right_h : left_h );
}

/* =============================================================================
 * Author: Elliott Rarden
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
 * Author: Elliott Rarden
 *
 * Description:
 * This funciton returns the smallest value in the tree
 * 
 * Paramaters:
 *   IN  hight - The hight of the tree
 * 
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
int getSmallestValue(int &file_loc)
{
	int val = 0;
	while(read_node(file_loc).left_child != RB_NULL)
	{
		file_loc = read_node(file_loc).left_child;
		val = read_node(file_loc).key_value;
	}
	return val;
}

/* =============================================================================
 * Author: Elliott Rarden
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
int getLargestValue(int &file_loc)
{
	int val = 0;
	while(read_node(file_loc).right_child != RB_NULL)
	{
		file_loc = read_node(file_loc).right_child;
		val = read_node(file_loc).key_value;
	}
	return val;
}