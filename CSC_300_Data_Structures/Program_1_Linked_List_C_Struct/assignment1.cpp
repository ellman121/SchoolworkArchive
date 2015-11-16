/*************************************************************************//**
 * Assignment 1 - Linked and STL Lists
 *
 * Elliott Rarden
 *
 * October 2, 2014
 * Professor: Dr. Ed Corwin
 * Course: CSC 300 - 10:00am
 * Location: McLaurey 205
 *
 * Program Information
 *
 * Details This program expects any number < 1000 of 32 bit integers from
 * standard input.  It will put them into a linked list, and bubble sort them
 *
 * Compiling Instructions:
 *      Use g++ to compile, uses C++11 Functions
 *****************************************************************************/
#include <iostream>
#include <iomanip>

using namespace std;

struct node
   {
   int data;
   node *next;
   };

typedef node * node_ptr;

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
void addToList(int x, node_ptr &head);
void bubbleSort(node_ptr &l);
bool issorted(node_ptr &l);

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 * This is the main function of a program to bubble sort a linked list of ints
 *
 * Paramaters:
 *   IN  - Number of command-line arguments
 *       - 2D Array containing argument values
 *
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
int main(int argc, char** argv)
   {
   int i; // Incrementers
   int j = 0;
   int n; // Number of numbers to be in the list
   int m; // Input buffer

   // Read in number of integers to put in the list
   cin >> n;

   do {
   // Create a new list
   node_ptr headpointer = new node;

   // Set up list for fun things later on
   headpointer->data = 0;
   headpointer->next = nullptr;

   // Fill the list
   for(i = 0; i < n; i++)
      {
      // Read in an integer fron cin and put it in the back of the list
      cin >> m;
      addToList(m, headpointer);
      }

   // Output whichever iteration we're on
   cout << "Case " << ++j << ":" << endl;

   // Sort the list
   bubbleSort(headpointer);

   // Delete the list - Memeory Leaks are bad
   while(headpointer != nullptr)
      {
      node_ptr current;
      current = headpointer;
      headpointer = headpointer->next;
      delete current;
      }

   } while(cin >> n); // Loop to the top if we have more integers to sort

   return 0;
   }

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 * This is the main function of a program to bubble sort a linked list of ints
 *
 * Paramaters:
 *   IN  - Number to add to the list
 *       - Pointer to the first node passed by reference
 *
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
void addToList(int x, node_ptr &head)
   {
   // First node case
   if(head->data == 0)
      {
      head->data = x;
      return;
      }

   // If were not at the end of the tree, go to it
   if(head->next != nullptr)
      {
      addToList(x, head->next);
      return;
      }
   else
      {
      // Create new node
      node_ptr temp = new (nothrow) node;

      // Allocation Error
      if(temp == nullptr)
         {
         cout << "Allocation error" << endl;
         return;
         }

      // Fill the new node
      temp->data = x;
      temp->next = nullptr;

      // Attach new node to end of list
      head->next = temp;
      }
   }

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 * This is the main function of a program to bubble sort a linked list of ints
 *
 * Paramaters:
 *   IN  - Number to add to the list
 *       - Pointer to the first node passed by reference
 *
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
void bubbleSort(node_ptr &l)
   {
   int size = 0; // Size of the list
   int i = 0; // Iterators
   int j = 0;
   int k = 0;
   int temp = 0;

   // A node that goes in front of the current
   node_ptr after = l;
   node_ptr current = l;
   node_ptr out = l;
   node_ptr temp2 = nullptr;

   // Get the size of the list
   while(after->next != nullptr)
      {
      size++;
      after = after->next;
      }

   // Set after to be the node in front of l (head)

   after = l->next;

   // Bubble sort
   for(i = 0; i < size; i++)
      {

      // Output the list each pass
      out = l;
      j = 0;
      while(out != nullptr)
         {
         cout << setw(4) << out->data;
         if((++j) % 10 == 0)
            {
            cout << endl;
            }
         out = out->next;
         }

      // Make things pretty by outputting a bunch of hyphens
      cout << endl << "----------" << endl;

      // Bubble sort
      for (j = 0; j <= size-1; j++)
         {
         current = l;
         // Walk to the current node
         k = 0;
         while(k < j)
            {
            current = current->next;
            k++;
            }
         after = current->next;

         // If out of order, swap values
         if(current->data > after->data)
            {
            temp = current->data;
            current->data = after->data;
            after->data = temp;
            }
         }

      // If the list is sorted, end
      if(issorted(l))
         {
         break;
         }
      }

   // Output the list each pass
   out = l;
   j = 0;
   while(out != nullptr)
      {
      cout << setw(4) << out->data;
      if((++j) % 10 == 0)
         {
         cout << endl;
         }
      out = out->next;
      }

   // Make things pretty by outputting a bunch of hyphens
   cout << endl << "----------" << endl;
   }

bool issorted(node_ptr &l)
{
   node_ptr temp = l;

   while(temp->next != nullptr)
   {
      if(temp->data > temp->next->data)
         return false;

      temp = temp->next;
   }
   return true;
}
