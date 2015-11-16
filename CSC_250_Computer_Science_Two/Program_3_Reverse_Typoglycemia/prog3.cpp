/*************************************************************************//**
 * @file 
 *
 * @brief This file contains the main and supporting funcitons for Program 3
 *
 * @mainpage Program 3 - Reverse Typoglycemia
 * 
 * @section course_section Course Information 
 *
 * @author Elliott Rarden
 * 
 * @date April 9, 2014
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
 * @details This program takes in a text file, reads in the contents, and will
 * unscramble the words found in the file.  It will then output the results to
 * the specified output file
 *
 * @section compile_section Compiling and Usage 
 *
 * @par Compiling Instructions: 
 *      Use either Visual Studio or G++ to compile.  Run from the command line
 * 
 * @par Usage: 
   @verbatim  
   c:\> Prog3.exe [message].txt [output].txt
   *NOTE* The dictionary must be in the current directory
   @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug
 * If a word is not in the dictionary it will not unscramble it!  
 * 
 * @todo 
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Mar 31, 2014  Created git repository and read in file/verified arguments
   Apr  1, 2014  Read in dictionary
   Apr  5, 2014  Permuted Strings and inserted into string vector
   Apr  6, 2014  Compared strings to the dictionary
   Apr  7, 2014  Output to command line preserving spacing
   Apr  9, 2014  Changed output to file preserving spacing
   Apr 10, 2014  Debugged in Visual Studio
   @endverbatim
 *
 *****************************************************************************/

#include "linkedlist.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
string checkWordAgainstDictionary(string word, linkedlist &dictionary);
bool isAlphaOmega(char ch);
bool readFile_linkedlist(const char *file, linkedlist &l);
void permuteString(char *str, int pos, char first, char last, vector<string> &array);
bool readFile_vector(const char *file, vector<string> &stgVector, vector<char> &spacingVector);
bool writeFile(const char *file, vector<string> outputVector, vector<char> whitespaceVector);

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This is the start of the progarm which takes two command line arguments.
 * The first is the message of scrambled words to be read, the second is a file
 * for output.  It will first read in the words from the file.  While ignoring
 * punctuation, it will permute the words.  It will compare each permutation to
 * the dictionary and will output the unscrambled word to the specified file.
 * This file will preserve whitespace from the original message
 *
 * @param[in] argc - the number of arguments from the command prompt.
 * @param[in] argv - a 2d array of characters containing the argument's values.
 *
 * @returns 0 Program fnished successfuly
 * @returns 1 Program encountered an error
 *****************************************************************************/
int main(int argc, char **argv)
{
	unsigned int i = 0, j = 0; //Incrementers
	linkedlist dictionary; //The dictionary to compare words against
	string word, temp, testString; //String primatives
	vector<string> messageIn, messageOut; //String vectors for the messages
	vector<char> whitespace; //Character vector to preserve whitespace form the input file

	messageIn.resize(0);
	messageOut.resize(0);
	whitespace.resize(0);

	//Verify commmand line arguments
	if(argc != 3)
	{
		cout << "INVALID USE" << endl;
		cout << "Usage: prog3.exe [messageFile] [destinationFile]" << endl;
		cout << "press enter to continue" << endl;
		cin.get();
		return 1;
	}

	if(!readFile_linkedlist("dictionary.txt", dictionary))
	{
		cout << "Dictoinary did not load" << endl;
		cout << "c:\\> Prog3.exe [message].txt [output].txt" << endl;
		cout << "*NOTE* The dictionary must be in the current directory" << endl;
		cin.get();
		return 1;
	}

	if(!readFile_vector(argv[1], messageIn, whitespace))
	{
		cout << "Input file failed" << endl;
		cout << "c:\\> Prog3.exe [message].txt [output].txt" << endl;
		cout << "*NOTE* The dictionary must be in the current directory" << endl;
		cin.get();
		return 1;
	}

	for(i = 0; i < messageIn.size(); i++)
	{
		testString = messageIn.at(i);
		if ((messageIn.at(i)).size() <= 3)
		{
			messageOut.push_back(messageIn.at(i));
		} else
		{
			messageOut.push_back(checkWordAgainstDictionary(messageIn.at(i), dictionary));
		}
	}

	if(!writeFile(argv[2], messageOut, whitespace))
	{
		cout << "Output file failed" << endl;
		cout << "c:\\> Prog3.exe [message].txt [output].txt" << endl;
		cout << "*NOTE* The dictionary must be in the current directory" << endl;
	}

	return 0;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function returns the first permutation of the string passed in against
 * the dictionary.  If no match is found, it returns the string unmodified
 *
 * @param[in] word The string to be permuted
 * @param[in] dictionary The dictionary to check permutatoins against (passed by reference)
 *
 * @returns string - the permuted string
 *****************************************************************************/
string checkWordAgainstDictionary(string word, linkedlist &dictionary)
{
	/*
	 *  Local Variables
	 */
	unsigned int i = 0, j = 0;
	int numPuncs;
	long int numPerms = 0;
	bool isCaped = false, wordFound = false;
	char punctuationStack[5] = {'\0'};
	char *wordAsArray = new (nothrow) char[word.size()+1]; 
	vector<string> permutationsVector;

	/*
	 *  Basic Modifications on local variables
	 */
	strcpy(wordAsArray, word.c_str()); //Get a char* out of a c++ string, just for you Schrader
	permutationsVector.resize(0); //Set the vector to size 0 so push_back works with the algorithm

	//First things first, get info about the word so we can reconstruct it later
	if(isAlphaOmega(wordAsArray[0]))
	{
		if(wordAsArray[0] >= 65 && wordAsArray[0] <= 90)
		{
			isCaped = true;
			wordAsArray[0] ^= 32; //Flip the case bit now that we know it's capd
		}
	}

	//Move all punctuation at the end of the string into a stack
	i = word.size() - 1; //We subtract one so we don't grab the null terminator
	numPuncs = 0; //Don't forget to put semicolons at the end of your statements
	while(!isAlphaOmega(wordAsArray[i]))
	{
		punctuationStack[j] = wordAsArray[i];
		wordAsArray[i] = '\0';
		numPuncs++;
		i--;
	}

	//First lets try to see if the word doesn't need modification.  e.g. is
	//already spelled correctly
	for(j = 0; j < dictionary.size(); j++)
	{
		if(word == dictionary.wordAt(i))
		{
			word = dictionary.wordAt(j);
			wordFound = true;
		}
	}

	//If the word isn't already spelled correctly, then we'll go ahead and permutate
	if(wordFound == false)
	{
		permuteString(wordAsArray, 1, wordAsArray[0], wordAsArray[strlen(wordAsArray) - 1], permutationsVector);

		//Now that all the preperation is done, we can permutate and compare things
		for(i = 0; i < permutationsVector.size(); i++)
		{
			for(int j = 0; j < dictionary.size(); j++)
			{
				if(permutationsVector.at(i) == dictionary.wordAt(j))
				{
					word = dictionary.wordAt(j);
					wordFound = true;
				}
			}
			if(wordFound)
				break;
		}
	}

	//If we didn't find the word or its permutations in the dictionary, 
	// put a '*' in front.
	if(wordFound == false)
		word.insert(0,1,'*');

	//Now we just have to reconstruct the word
	if(isCaped == true)
	{
		word[0] ^= 32; //flip the case bit
	}

	//Reappend punctuation
	while(numPuncs != 0)
	{
		word.push_back(punctuationStack[numPuncs]);
		numPuncs--;
	}

	//Now were done, clean up and return the word
	delete[] wordAsArray;
	return word;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function returns true if the passed in char is A-Z or a-z
 *
 * @param[in] ch The character to be tested
 *
 * @returns true - the character is A-Z or a-z
 * @returns false - the character is not A-Z or a-z
 *****************************************************************************/
bool isAlphaOmega(char ch)
{
	if(ch >= 65 && ch <= 90) //Upper Case A-Z
		return true;

	if(ch >= 97 && ch <= 122) //Lower Case a-z
		return true;

	return false;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function permutes the char* passed in and stores each permutation with
 * the first and last characters (arguments) specified.  The pos argument can be
 * used to specify how many characters in front of the string to ignore.  
 *
 * @param[in] str The string to permute
 * @param[in] pos The starting position of the permutation (e.g. how many front characters to ignore)
 * @param[in] first Specify the first character of the results (e.g. select your outputs)
 * @param[in] last Specify the last character of the results (e.g. select outpus)
 * @param[in, out] array A string vector of the permutations matching the specified criteria 
 *****************************************************************************/
void permuteString(char *str, int pos, char first, char last, vector<string> &array)
{
	unsigned int i;

	//If we have a match to the specified criteria
	if(pos == strlen(str) && str[0] == first && str[strlen(str) - 1] == last)
	{
		array.push_back(str);
		array.back().resize(strlen(str));
		array.back() = str;
		return;
	}

	for(i = pos; i < strlen(str); i++)
	{
		swap(str[pos], str[i]);
		permuteString(str, pos + 1, first, last, array);
		swap(str[pos], str[i]);
	}
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function reads in each word in a file and inserts them into a sorted linked list
 *
 * @param[in] file The file to read
 * @param[in] l The linked list to insert into (passed by reference)
 *
 * @returns true - the file was read and inserted into the list
 * @returns false - the file failed to open
 *****************************************************************************/
bool readFile_linkedlist(const char* file, linkedlist &l)
{
	string tmp;
	ifstream fin;

	fin.open(file, ios::in);
	if(fin.is_open())
	{
		//Read the file
		while(fin >> tmp)
		{
			l.insert(tmp);
		}

		//Close the file and return true
		fin.close();
		return true;
	}

	cout << "file failed to open" << endl;
	//Return false if the file didn't open correctly
	return false;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function reads in a file and inserts each word into a string vector
 *
 * @param[in] file The file to read
 * @param[in] stgVector The string vector to read into
 * @param[in] spacingVector A char vector to store all whitespace characters between words
 *
 * @returns true - the file was read and inserted into the list
 * @returns false - the file failed to open
 *****************************************************************************/
bool readFile_vector(const char *file, vector<string> &stgVector, vector<char> &spacingVector)
{
	int i = 0;
	string tmp, temp;
	ifstream fin;

	fin.open(file, ios::in||ios::binary);
	if(fin.is_open())
	{
		//Read the file into a vector
		while (fin >> tmp)
		{
			stgVector.push_back(tmp);
			spacingVector.push_back(fin.peek());
		}

		//Close the file and return true
		fin.close();
		return true;
	}

	//Return false if the file didn't open correctly
	return false;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function writes a vector to a file
 *
 * @param[in] file The file to write to
 * @param[in] outputVeector The vector to be written to the file
 * @param[in] whitespaceVector The whitespace from the original file
 *****************************************************************************/
bool writeFile(const char *file, vector<string> outputVector, vector<char> whitespaceVector)
{
	unsigned int i = 0;
	ofstream fout;

	fout.open(file, ios::out);

	if(fout.is_open())
	{
		//Output each word to the file
		for(i = 0; i < outputVector.size(); i++)
		{
			fout << outputVector.at(i);

			//We need to ignore the last entry in the whitespace vector to avoid
			//random characters being printed
			if(i < whitespaceVector.size() - 1)
				fout << whitespaceVector.at(i);
		}

		//Close the file and return true
		fout.close();
		return true;
	}

	//Return false if the file didn't open correctly
	return false;
}