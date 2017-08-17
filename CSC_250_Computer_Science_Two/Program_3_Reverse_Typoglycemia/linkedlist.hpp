/*************************************************************************//**
 * @file linkedlist.h
 *
 * @brief this file contains the class definition for the linkedlist class
 ****************************************************************************/
#ifndef __linkedlist_h
#define __linkedlist_h

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

/*!
 * @brief The class definition for the linkedlist
 */
class linkedlist
{
	//Public member funcitons
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
		bool insert( string str );

		/*!
		* @brief removes a word from the linked list. Returns true if successful
		*/
		bool remove( string str );

		/*!
		* @brief finds a word in the linked list. Returns true if sucessful
		*/
		bool find( string str );

		/*!
		* @brief Increases the frequency count of a word. Returns true if sucessful
		*/
		bool incrementFrequency( string str );

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
		void printToFile(ostream &out);

		/*!
		* @brief Outputs the list to the command line in alphabetical order
		*/
        void printToCommandline();

		/*!
		* @brief Returns the word at a given position
		*/
		string wordAt(int pos);

	//Privatie variables
    private:
		/*!
		 * @brief Structure for a node. Keeps track of the frequency for the word.
		 * Stores the word. Also keeps a pointer to the next word in the list.
		*/
        struct node
        {
            int frequencyCount;
            string word;
            node *next;
        };

		/*!
		 * @brief Points to the first node in the list
		*/
        node *headptr;
};

#endif