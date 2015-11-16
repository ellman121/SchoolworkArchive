/*************************************************************************//**
 * @file 
 *
 * @brief This file contains program 2 using the STL List Class
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
 * @bug It will occassinally insert extra new lines into the outupt file,
 * specifically when there are large differences in numbers of instances
 * of a word.
 * 
 * @todo Fix the bug above
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Mar 25, 2011  Created Git Repo for the stl version of the code
   Mar 26, 2014  Finished reading in and modifying strings
   Mar 27, 2014  Finished writing to the file
   Mar 28, 2011  Finished the DoxyGen and bug fixed
   @endverbatim
 *
 *****************************************************************************/
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;

/*!
 * @brief Defines an item for use in the list
 *
 * @details item.word is a string that contains a word.  item.frequency is
 * and incrementable integer containing the number of occourances of that word
 */
struct item
{
	int frequency;
	string word;
};

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
bool wordIsInList(string word, list<item> &l);
bool compare2ItemsByWord(item &l, item &r);
bool compare2ItemsByFrequency(item &l, item &r);
void readFileAndInsertToList(string file, list<item> &l);
void outputToFile(list<item> &l, string outputFile);
void stripPunctuationAndDowncase(string &word);
string numSpaces(int numberOfSpacesToReturn);

/**************************************************************************//**
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison
 *
 * @par Description:
 * This is the start of the progarm which takes two command line arguments.
 * The first is the story to be read, the second is the file to output to.  It
 * will first read in the words from the file and then strip all punctuation
 * from the sides of the words, leaving all punctuation in the middle of the a
 * word.  Then, it will insert unique words into an STL List.  If the word is
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
int main(int argc, char **argv)
{
	//Variables
	int i, length, numWords = 0; //Define numWords to be 0, since we don't have any words yet
	string newWord, story, outputFile;
	list<item> myList;

	//Validate Input
	if(argc != 3)
	{
		cout << "ERROR: Invalid Arguments" << endl;
		cout << "USAGE: prog2stl.exe [story].txt" << endl;
		cout << "Press enter to continue" << endl;
		cin.get();
		exit(0);
	}

	//Assign variables
	story = argv[1];
	outputFile = argv[2];

	//Read the file and inert the words into list
	readFileAndInsertToList(story, myList);
	
    //Sort the list by Frequency
    myList.sort(compare2ItemsByFrequency);

    //Write to file
	outputToFile(myList, outputFile);
	 
	 return 0;
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * Returns true if the word is found in the list
 * 
 * @param wordToFind The word to search for in the list
 * @param l The list to search through (passed by reference)
 ****************************************************************************/ 
bool wordIsInList(string wordToFind, list<item> &l)
{
	list<item>::iterator i;
	for(i = l.begin(); i != l.end(); i++)
	{
		if(i->word == wordToFind)
		{
			return true;
		}
	}
	return false;
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * Compares two items by their respective words.  Returns true if
 * the left is greater than the right, flase otherwise.
 * 
 * @param l The left index (passed by reference)
 * @param r The right index (passed by reference)
 ****************************************************************************/ 
bool compare2ItemsByWord(item &l, item &r)
{
	if((l.word).compare((r.word)) <= 0)
	{
		return true;
	}
	return false;
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * Compares two items by their respecitve frequencies.  Returns true if the left
 * is greater than the right, false if the right is greater than the left.  If
 * the frequencies are equal, it will sort the items by their words instead.
 * 
 * @param l The left index (passed by reference)
 * @param r The right index (passed by reference)
 ****************************************************************************/ 
bool compare2ItemsByFrequency(item &l, item &r)
{
	if((l.frequency) > (r.frequency))
	{
		return true;
	} 
	else if((l.frequency) == (r.frequency))
	{
		return compare2ItemsByWord(l, r);
	}
	return false;
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * This function reads in the specified text file and then inserts the words in
 * it into a STL List of items.
 * 
 * @param file The file to be read in
 * @param l The list into which the words will be inserted (passed by reference) 
 ****************************************************************************/ 
void readFileAndInsertToList(string file, list<item> &l)
{
	ifstream fin;
	string newWord;
	item tempItem;
	list<item>::iterator iterator;
	int i, length;

	//Open the story
	fin.open(file);
	if(!fin)
	{
		cout << "File failed to open" << endl;
		cout << "Press enter to continue" << endl;
		cin.get();
		exit(0);
	}

	//Read in the file word by word, strip punctuation, and insert into list
	while(fin >> newWord)
    {
    	length = newWord.size();

    	//Clean up the words so they can be compared easily
		stripPunctuationAndDowncase(newWord);
		tempItem.word = newWord;

    	//Assign the word to a temporary variable for insertion
    	tempItem.word = newWord;

    	//If the word is in the list, increase it's frequency, else insert it
    	if(wordIsInList(newWord, l))
    	{
    		for(iterator = l.begin(); iterator != l.end(); iterator++)
    		{
    			if(iterator->word == newWord)
    			{
    				iterator->frequency++;
    			}
    		}
    	} else
    	{
    		tempItem.frequency = 1;
    		l.push_back(tempItem);
    	}
    }
    fin.close();
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * This funciton outputs the specified formatted list to a text file.  The list
 * should be sorted by frequency before bing passed in
 * 
 * @param l The list to be written to the flie (passed by reference)
 * @param outputFile The file to write to
 ****************************************************************************/ 
void outputToFile(list<item> &l, string outputFile)
{
	ofstream fout;
	int i=0, maxFrequency = 0, needNewLine = 1;
	bool header = false;
	list<item>::iterator iterator;
	//open output file and check to make sure it opens
	fout.open(outputFile, ios::out|ios::trunc);
	if(fout.is_open())
	{
		//If opened output results to file
		//First find largest frequency
		for(iterator = l.begin(); iterator != l.end(); iterator++)
    	{
    		if(iterator->frequency > maxFrequency)
    		{
    			maxFrequency = iterator->frequency;
    		}
    	} 
    	//using the largest frequency as a stopping point, output to file
		for(i=1; i <= maxFrequency; i++)
		{
			for(iterator = l.begin(); iterator != l.end(); iterator++)
    		{
    			if(iterator->frequency == i)
    			{
    				if(header == false)
    				{
    					//output header
						fout << "****************************************" << endl;
						fout << "* Frequency Count: " << i; //frequency of words
						fout << numSpaces(18) << "*" << endl; 
						fout << "****************************************" << endl;
						header = true;
    				}

    				fout << iterator->word << numSpaces(31 - (iterator->word).length());
    				if(needNewLine % 2 == 0)
    				{
    					fout << endl;
    					needNewLine = 1;
    				} else
    				{
    					needNewLine++;
    				}
				}
			}
			needNewLine = 1;
			header = false;
			fout << endl;
		}
	}
	else
	{
		//output error message if failed to open
		cout << "Output file failed to open." << endl;
	}
	fout.close();
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * This function takes in a string and will strip punctuation from the sides of
 * the string, but not from the middle.  It will then downcase the word.
 * 
 * @param word The string to be modified (passed by reference)
 ****************************************************************************/ 
void stripPunctuationAndDowncase(string &word)
{
	int i = 0;

	//Strip punctuation from front of word, reverse the word, and repeat
	while(ispunct(word.back()))
	{
		word.pop_back();
	}
	reverse(word.begin(), word.end());
	while(ispunct(word.back()))
	{
		word.pop_back();
	}
	reverse(word.begin(), word.end());

	//Set all characters to lower case
	for(i = 0; i < word.size(); i++)
	{
			word.at(i) |= 32;
	}
}

/*************************************************************************//** 
 * @author Elliott Rarden, Alex Stulken, Caleb Jamison 
 * 
 * @par Description: 
 * This function retruns a string with with the specified number of spaces
 * e.g. numSpaces(5) will return "     "
 * 
 * @param numberOfSpacesToReturn The number of spaces in the string
 ****************************************************************************/ 
string numSpaces(int numberOfSpacesToReturn)
{
	string retValue = "";
	for(int i = 0; i <= numberOfSpacesToReturn; i++)
	{
		retValue += " ";
	}
	return retValue;
}
