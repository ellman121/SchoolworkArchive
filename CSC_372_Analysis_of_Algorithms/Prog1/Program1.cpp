// Program1.cpp
// Elliott Rarden
// First assignment for Analysis of Algorithms

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*
 *  GLOBAL LOOK UP TABLES
 */
vector<string> stringLUT;
vector<bool> answersLUT;

/*
 *  FUNCTION PROTOTYPES
 */
bool stringIsRepeatedPairs(string testString);

/* int main()
 *
 * Author: Elliott Rarden
 *
 * Description: Handle file I/O and call functions
 *
 * Parameters:
 * 		Input:		argc - Argument count
 * 					argv - argument
 * 		Returns:	An integer denoting any errors (0 for no errors)
 */
int main(int argc, char *argv[])
{
	int caseNumber = 0; // The current case number
	ifstream fin; // Input file handler
	ofstream fout; // Output file handler

	string testString;

	// Open our input and output files
	fin.open(argv[1]);
	fout.open("pairs.out");

	// Clear the lookup tables
	stringLUT.clear();
	answersLUT.clear();

	// While we can read a string
	while(fin >> testString)
	{
		// Increase our case number
		caseNumber++;

		// String of odd length can't be repeated pairs
		if (testString.size() % 2 == 1)
		{
			fout << caseNumber << " no" << endl;
			continue;
		}

		// If the whole string is a repeat, then we can just return this up front.
		if (testString.substr(0, testString.size()/2) == testString.substr(testString.size()/2))
		{
			stringLUT.push_back(testString);
			answersLUT.push_back(true);
			fout << caseNumber << " yes" << endl;
			continue;
		}

		// Output our decision
		if (stringIsRepeatedPairs(testString))
		{
			fout << caseNumber << " yes" << endl;
		}
		else
		{
			fout << caseNumber << " no" << endl;
		}
	}

	// Close or input and output files
	fin.close();
	fout.close();

	return 0;
}

/* bool stringIsRepeatedPairs(string testString)
 *
 * Author: Elliott Rarden
 *
 * Description: Recursive function to decide if a string is a repeatition of pairs
 *
 * Parameters:
 * 		Input:		testString - The string to be tested
 * 		Returns:	An bool denoting ture or false
 */
bool stringIsRepeatedPairs(string testString)
{
	int length = testString.size();
	bool guess;

	// Look up un the table
	for (int i = 0; i < stringLUT.size(); ++i)
	{
		if (stringLUT[i] == testString)
		{
			return answersLUT[i];
		}
	}

	// If the whole string is a repeat, then we can just return this up front.
	if (testString.substr(0, testString.size()/2) == testString.substr(testString.size()/2))
	{
		stringLUT.push_back(testString);
		answersLUT.push_back(true);
		return true;
	}

	// Test the last two, three, etc.
	for(int i = 2; i <= length; i+=2)
	{
		// Look at the first i characters
		string temp = testString.substr(0, i);

		// Left and right halves
		string left = temp.substr(0, temp.size() / 2);
		string right = temp.substr(temp.size() / 2);

		// If our substring of i characters is a repetition
		if(left == right)
		{
			// Recurse on the rest of the string
			// If the rest of the string is valid, return true
			if((testString.size() == 0) || (stringIsRepeatedPairs(testString.substr(i))))
			{
				// Insert into the table and return
				stringLUT.push_back(testString);
				answersLUT.push_back(true);
				return true;
			}
			// Else, we increment i and try again
		}

	}

	// Insert into the table and return
	stringLUT.push_back(testString);
	answersLUT.push_back(false);
	return false;

}
