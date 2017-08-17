#ifndef __rb_tree_h
#define __rb_tree_h

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct rb_tree_node
  {
  int  key_value;
  int  left_child;    // child file address
                      // [record index, not byte address]
  int  right_child; 
  bool left_red;      // true if left child is red
  bool right_red;     // true if right child is red

  bool pad1;          // padding since compiler allocates two extra bytes
  bool pad2;          // padding since compiler allocates two extra bytes

  // may need filler to force zeros in any padding of the struct
  int  file_loc;      // location on file (record index)
  };

const int RB_NULL = -1;
const int RB_NODE_SIZE = sizeof(rbtree_node);
typedef rbtree_node* rbtree_ptr;

// File i/o for the rb_tree
fstream file;

/******************************************************************************
 *                         Function Prototypes                                *
 ******************************************************************************/

/* ==== FAMILY FUNCTIONS ==== */
rbtree_node grandparent(rbtree_node n);
rbtree_node left_child(rbtree_node n);
rbtree_node new_node(int file_loc = -1, int val = -1, bool color = RED, int left = -1, int right = -1);
rbtree_node parent(rbtree_node n);
rbtree_node right_child(rbtree_node n);
rbtree_node sibling(rbtree_node n);
rbtree_node uncle(rbtree_node n);

/* ==== INSERT FUNCTIONS ==== */
void insert(int val, int &nodesWritten);
void insert_case1(rbtree_node &n, int &nodesWritten);
void insert_case2(rbtree_node &n, int &nodesWritten);
void insert_case3(rbtree_node &n, int &nodesWritten);
void insert_case4(rbtree_node &n, int &nodesWritten);
void insert_case5(rbtree_node &n, int &nodesWritten);

/* ==== ROTATE FUNCTIONS ==== */
void rotate_left(rbtree_node &p, int &nodesWritten);
void rotate_right(rbtree_node &p, int &nodesWritten);
void swap_node(rbtree_node &dest, rbtree_node &source, int &nodesWritten);

/* ==== FILE I/O FUNCTIONS ==== */
void readFile(ostream &out);
rbtree_node read_node(int file_loc);
rbtree_node read_root();
void write_node(rbtree_node &n, int &nodesWritten);
void write_node(int file_loc, rbtree_node &n, int &nodesWritten);

#endif
