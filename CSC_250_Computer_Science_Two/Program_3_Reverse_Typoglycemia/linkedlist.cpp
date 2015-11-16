/*************************************************************************//**
 * @file
 *
 * @brief This file contains the method implementation for the 
 * linkedlist class.  
 ****************************************************************************/
#include "linkedlist.h"

/*************************************************************************//** 
 * @author Elliott Rarden
 * 
 * @par Description: 
 * The constructor will allow for you to initialize the list.  The new list is
 * compeletely empty and will have headptr set to nullptr.
 ****************************************************************************/ 
linkedlist::linkedlist()
{
	headptr = nullptr;
}

/*************************************************************************//** 
 * @author Jack Weiss 
 * 
 * @par Description: 
 * The destructor for the linkedlist class first deletes all the nodes in the
 * list and then exits.
 ****************************************************************************/ 
linkedlist::~linkedlist()
{
	node *temp;
	while(headptr != nullptr)
	{
		temp = headptr;
		headptr = headptr->next;
		delete temp;
	}

	delete headptr;
}

/**************************************************************************//**
 * @author Elliott Rarden, Alexander Stulken, and Caleb Jamison
 *
 * @par Description:
 * This function returns true if the word was inserted into the list, 
 * false otherwise
 *
 * @param[in] str The string to be inserted into the list
 *
 * @returns true - the string was inserted
 * @returns false - the string was not inserted
 *****************************************************************************/
bool linkedlist::insert( string str )
{
	node* tmp = nullptr;
	node* new_node = new (nothrow) node;
	if( new_node == nullptr )
		return false;

	new_node->word = str;
	new_node->frequencyCount = 1;

	if( headptr == nullptr || headptr->word >= new_node->word )
	{
		new_node->next = headptr;
		headptr = new_node;
	}
	else
	{
		tmp = headptr;
		while( tmp->next != nullptr && tmp->next->word < new_node->word )
			tmp = tmp->next;
		new_node->next = tmp->next;
		tmp->next = new_node;
	}

	return true;
}

/**************************************************************************//**
 * @author Elliott Rarden and Caleb Jamison
 *
 * @par Description:
 * This function returns true if the string is removed from the list, false
 * otherwise
 *
 * @param[in] str The string to be removed
 *
 * @returns true - the string is found
 * @returns false - the string is not found
 *****************************************************************************/
bool linkedlist::remove( string str )
{
	node *current = nullptr;
	node *previous = nullptr;

	current = headptr;

	while(current != nullptr)
	{
		if((current->word).compare(str) == 0)
		{
			previous = current->next;
			delete current;
			return true;
		}
		previous = current;
		current = current->next;
	}
	return false;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function returns true if the specified string is found in the list
 *
 * @param[in] str The string to be found in the list
 * 
 * @returns true - the string is found
 * @returns false - the string is not found
 *****************************************************************************/
bool linkedlist::find( string str )
{
	node *temp = nullptr;

	temp = headptr;

	while(temp != nullptr)
	{
		if(temp->word == str)
			return true;
		temp = temp->next;
	}
	return false;
}

/**************************************************************************//**
 * @author Caleb Jamison
 *
 * @par Description:
 * This function returns true if the frequiency was incremented
 *
 * @returns true - the string is found and increment
 * @returns false - the string is not found
 *****************************************************************************/
bool linkedlist::incrementFrequency( string str )
{
	node *temp = nullptr;

	temp = headptr;

	while(temp != nullptr)
	{
		if(temp->word == str)
		{
			temp->frequencyCount++;
			return true;
		}

		temp = temp->next;
	}
	return false;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function returns true if the linked list is empty
 *
 * @returns true - the list is empty
 * @returns false - the list is not empty
 *****************************************************************************/
bool linkedlist::isEmpty()
{
	//Unless (headptr == nullptr)
	if( !(headptr == nullptr) ) 
		return false;

	//Always safer to return true as a default.  
	return true;
}

/**************************************************************************//**
 * @author Elliott Rarden and Caleb Jamison
 *
 * @par Description:
 * This function returns the highest frequency.  Note, does not return the entry
 * in the list, just the integer itself.
 *
 * @returns 0 - List is empty
 * @returns !0 - The Highest occuring word's count
 *****************************************************************************/
int linkedlist::getMaxFrequency()
{
	int maxFrequency = 0;
	node *temp = nullptr;
	temp = headptr;

	if(isEmpty())
		return 0;

	while(temp != nullptr)
		if(temp->frequencyCount > maxFrequency)
			maxFrequency = temp->frequencyCount;

	return maxFrequency;
}

/**************************************************************************//**
 * @author Elliott Rarden and Caleb Jamison
 *
 * @par Description:
 * This function returns the number of nodes in the list.  Includes repeats
 *
 * @returns 0 - List is empty
 * @returns !0 - The number of nodes in the list
 *****************************************************************************/
int linkedlist::size()
{
	int numNodes = 0;
	node *temp = nullptr;
	temp = headptr;

	while(temp != nullptr)
	{
		numNodes++;
		temp = temp->next;
	}

	return numNodes;
}

/*************************************************************************//** 
 * @author Alexander Stulken, Elliot rarden, Caleb Jamison
 * 
 * @par Description: 
 * This funciton outputs the specified formatted list to a text file.  The list
 * should be sorted by frequency before bing passed in
 * 
 * @param[in] out An ostream
 ****************************************************************************/
void linkedlist::printToFile( ostream &out )
{
	int i;
	int maxFrequency = 0, needNewLine = -1;
	bool header = false;
	node* tmp;
	tmp = headptr;
	//If opened output results to file
	//First find largest frequency
	while( tmp != nullptr )
	{
		if( tmp->frequencyCount > maxFrequency )
			maxFrequency = tmp->frequencyCount;
		tmp = tmp->next;
	}
	//using the largest frequency as a stopping point, output to file
	for(i=1; i <= maxFrequency; i++)
	{
		//reset tmp to headptr
		tmp = headptr;
		while( tmp != nullptr )
		{
			if(tmp->frequencyCount == i)
			{
				if( !header )
				{
					//output header
					out << "****************************************" << endl;
					out << "Frequency Count: " << i << endl; //frequency of words
					out << "****************************************" << endl;
					header = true;
					needNewLine = 1;
				}
				out << setw(20) << left << tmp->word;
				if( ( needNewLine % 2 == 0 ) )
				{
					out << endl;
					needNewLine = 1;
				}
				else
				{
					needNewLine++;
				}
			}
			tmp = tmp->next;
		}
		if( tmp == nullptr && header )
			out << endl << endl;
		needNewLine = 1;
		header = false;
	}
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * Prints the linkedlist to the command line, one word per line
 *****************************************************************************/
void linkedlist::printToCommandline()
{
	node *temp = nullptr;
	temp = headptr;

	while(temp->next != nullptr)
	{
		cout << temp->word << endl;
		temp = temp->next;
	}
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function returns the string at the specified position in the list
 *
 * @returns Empty_String The list is empty or the position is above the bounds
 * @returns Some_String The String at the specified position
 *****************************************************************************/
string linkedlist::wordAt(int pos)
{
	int i = 0;
	node *temp = headptr;

	if(temp == nullptr)
	{
		return "";
	}
	if(pos > this->size())
	{
		return "";
	}

	while(temp != nullptr && i != pos)
	{
		i++;
		temp = temp->next;
	}
	if ( temp == nullptr )
		return "";
	return temp->word;

}