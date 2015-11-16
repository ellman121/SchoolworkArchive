/*************************************************************************//**
 * @file 
 *
 * @brief This file contains program 2 using our linked list class
 *
 * @mainpage Program 2 - Counting Words
 * 
 * @section course_section Course Information 
 *
 * @author Elliott Rarden, Alexander Stulken, Caleb Jamison
 * 
 * @date March 28, 2014
 * 
 * @par Professor: 
 *         Roger Schrader
 * 
 * @par Course: 
 *         CSC 250 - M001 - 11:00am
 * 
 * @par Location: 
 *         Classroom Building - 204W
 *
 * @section program_section Program Information 
 * 
 * @details This program takes in a text file, and counts the occournaces of
 * each word in the file.  It will then output the results into a formatted
 * text file which the user specifies in the command prompt.
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      Use either Visual Studio or G++ to compile.  Run from the command line
 * 
 * @par Usage: 
   @verbatim  
   c:\> Prog3.exe [story].txt [output].txt
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug
 * 
 * @todo 
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Mar  4, 2014  Created Git Repo for our linked list code
   Mar  8, 2014  Finished linkedlist.h
   Mar 17, 2014  Started linkedlist.cpp and prog2.cpp and implemented some functions
   Mar 20, 2014  Finished most of the functions except the insert and output
   Mar 24, 2014  Wrote main and the pathfinding inside.
   Mer 28, 2014  Finshed rest of program and Doxygen
   @endverbatim
 *
 *****************************************************************************/
#include <string>
#include <vector>
#include <algorithm>
#include "linkedlist.h"

using namespace std;

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
bool readFile( const char* file, vector<string>* contents );
void conditionString( vector<string>* words);
void insertWords( vector<string> words, linkedlist* ll );
void outputToFile(linkedlist l, string outputFile);

/**************************************************************************//**
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison
 *
 * @par Description:
 * This is the start of the progarm which takes two command line arguments.
 * The first is the story to be read, the second is the file to output to.  It
 * will first read in the words from the file and then strip all punctuation
 * from the sides of the words, leaving all punctuation in the middle of the a
 * word. Then, it will insert unique words into a Linked List.  If the word is
 * already in the list, it will increment that words frequency.  The program
 * then sorts the list and outputs the frequencies and corrosponding words to
 * the specified output file.
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the arguments.
 *
 * @returns 0 Program fnished successfuly
 *
 *****************************************************************************/
int main( int argc, char **argv )
{
	//Variables
	vector<string> words;
	linkedlist ll;

	if(argc != 3)
	{
		cout << "INVALID USE: INCORRECT ARGUMENTS" << endl;
		cout << "USAGE: prpg2.exe [story].txt results.txt" << endl;
		return 0;
	}

	//Read in the story
	readFile( argv[1], &words );

	//Remove punctuation
	conditionString( &words );

	//Insert all the words into a linked list
	insertWords( words, &ll );

	ofstream fout;
	fout.open( argv[2], ios::out );

	//Print ll
	ll.print(fout);
	fout.close();
}	

/**************************************************************************//**
 * @author Elliott Rarden and Caleb Jamison
 *
 * @par Description: Reads in a file full of words into strings for processing
 *
 * @param[in] file The file to be opened and read
 *****************************************************************************/
bool readFile( const char* file, vector<string>* contents )
{
	string tmp;

	ifstream fin;
	fin.open( file, ios::in );
	if ( fin.is_open( ) )
	{
		//Read the file
		while( !fin.eof() )
		{
			fin >> tmp;
			contents->push_back(tmp);
		}

		//close the file and return the array
		fin.close( );

		return true;
	}
	return false;
}

/**************************************************************************//**
 * @author Caleb Jamison
 *
 * @par Description: Removes punctuation and lowercases the letters in words
 *
 * @param[in] file The array to remove punctuation from
 *****************************************************************************/
void conditionString( vector<string>* vect )
{
	unsigned int i, j;

	for( i = 0; i < vect->size(); i++)
	{
		while( ispunct( vect->at( i ).front( ) ) )
			vect->at( i ).erase( vect->at( i ).begin( ) );
		while( ispunct( vect->at( i ).back( ) ) )
			vect->at( i ).erase( vect->at( i ).end( ) - 1 );
		for( j = 0; j < vect->at( i ).size(); j++ )
			if( isupper(vect->at(i)[j]) )
				vect->at(i)[j] = vect->at(i)[j] + abs('a' - 'A');
	}
}

/**************************************************************************//**
 * @author Elliott Rarden, Caleb Jamison, Alexander Stulken
 *
 * @par Description: Inserts a word into the linked list by creating a node and placing
 * and placing it into the correct place on the list.
 *
 * @param[in] Word to be inserted into the list and the linked list.
 *****************************************************************************/
void insertWords( vector<string> words, linkedlist* ll )
{
	unsigned int i;

	for( i = 0; i < words.size(); i++)
	{
		if( !ll->find(words.at(i)) )
		{
			if( !(ll->insert( words.at(i) ) ) )
				cout << "failed to insert node" << endl;
		}
		else
			ll->incrementFrequency( words.at(i) );
	}
}

