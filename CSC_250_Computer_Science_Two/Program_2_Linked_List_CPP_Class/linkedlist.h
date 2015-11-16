/*************************************************************************//**
 * @file 
 *
 * @brief this file contains the basic information for prog2.cpp and linkedlist.cpp.
 * It should be included with prog2.cpp and linkedlist.cpp
 ****************************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

/*!
 * @brief The linkedlist class stores all the functions needed for the linked list.
 */
class linkedlist
{
	public:
		/*!
		* @brief Constructor for the linked list
		*/
		linkedlist();
		/*!
		 * @brief Deconstructor for the linked list
		*/
		~linkedlist();
 
		/*!
		* @brief inserts a word into the linked list. Returns true if sucessful.
		*/
		bool insert( string word );
		/*!
		* @brief removes a word from the linked list. Returns true if successful
		*/
		bool remove( string word );
		/*!
		* @brief finds a word in the linked list. Returns true if sucessful
		*/
		bool find( string word );
		/*!
		* @brief Increases the frequency count of a word. Returns true if sucessful
		*/
		bool incrementFrequency( string word );
		/*!
		* @brief Returns true if a node is empty.
		*/
		bool isEmpty();
		/*!
		* @brief Returns the manimum frequency of a word from the linked list.
		*/
		int getMaxFrequency();
		/*!
		 * @brief Returns the number of nodes in the list.
		*/
		int size();
		/*!
		* @brief Outputs the list into a file formatted correctly.
		*/
		void print( ostream &out ); 
 
	private:
		/*!
		 * @brief Structure for a node. Keeps track of the frequency for the word.
		 * Stores the word. Also keeps a pointer to the next word in the list.
		*/
		struct node
		{
			int frequencyCount;
			string word;
			node* next;
		};
		node* headptr;
};