/*************************************************************************//**
 * @file 
 *
 * @brief Implementations for all of the mp3 tag extraction, modification,
 *        and writing functions
 ****************************************************************************/

#include "functions.h"


/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function receives an array of strings and a reference of an mp3 tag.  It
 * will then call other functions to read in a tag, modify it, and then write it
 * to the file.
 *
 * @param[in] arguments - array of strings containing the arguments from the 
 *                        command line
 * @param[in] newTag    - an empty reference to an mp3 tag
 *
 * @returns none
 *****************************************************************************/
void changeFile(string arguments[15], mp3Tag newTag)
{
	ifstream fin;
	ofstream fout;

	// Perform the necessary tasks - read in the tag, modify it, and write it
	readFile(fin, arguments[0], newTag);
	modifyTag(arguments, newTag);
	writeFile(fout, arguments[0], newTag);
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function prints out the usage statement, then exits the program
 *
 * @returns none
 *****************************************************************************/
void exitWithError()
{
	// First, print out a new line, then print the usage statement
	// ask the user to press enter to continue, lastly, exit the program
	cout << endl;
	printUsageStatement();
	cout << "Press enter to continue";
	cin.clear();
	cin.get();
	exit(0);
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function takes in a string, and extracts an integer value from it.  For
 * example, if you passed in the string "153", it would return the int 153
 *
 * @param[in] str - A string containing an integer value
 *
 * @returns The integer extracted from the string
 *****************************************************************************/
int extractIntFromString(string str)
{
    stringstream ss;
    int returnValue;

    ss << str;
    ss >> returnValue;   //convert string into int and store it in "returnValue"
    ss.str("");          //clear the stringstream
    ss.clear();          //clear error flags because "42" is not a valid stream
    return returnValue;  //return the value
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function receives two arrays of strings and an integer.  The function 
 * formats the argument array with values passed in by oldArray.
 *
 * @param[in] oldArray  - array of strings containing the arguments
 * @param[in, out] formattedArray - formattedArray comes in empty, and is filled
 *                                  with the arguments in a specific manner
 * @param[in] numberOfIterations - iterator used with the for loop
 *
 * @returns none
 *****************************************************************************/
void formatArgumentArray(string oldArray[20], 
						 string formattedArray[15], int numberOfIterations)
{
	int i;
  /*
   * Array of boolean values.  Indexes are as follows
   * 0 is constantly false
   * 1 is songName
   * 2 is artistName
   * 3 is albumName
   * 4 is comment
   * 5 is year
   * 6 is genre
   * 7 is track
   * 
   * The array is used to 0 out strings later on in this function
   */
	bool flagArray[8] = {false, false, false, false, 
						 false, false, false, false};

  /* Go through oldArray, looking for any argument keys (-s, -t, etc.)
   * Each key has a specific spot in the formattedArray, see mp3.cpp for details.
   * Ignore the 0th index which will hold the file.mp3
   */
	organizeArray(numberOfIterations, oldArray, formattedArray, flagArray);

	// set all values of the array that don't contain changes to the string "0"
	// start at i = 1 because flagArray[0] is constaly false
	for(i = 1; i < 8; i++)
	{
		int reference = 2 * i - 1 ;
		if(flagArray[i] = false)
		{
			formattedArray[reference] = "0";
			formattedArray[reference +1] = "0";
		}
	}

	// Set the 0th index to the name of the file to be modified by parsing
	// through the oldArray to find a .mp3  If not found, assume that a direcotry
	// was passed in and do nothing
	for (i = 0; i < numberOfIterations; i++)
	{
		if(oldArray[i].find(".mp3") != string::npos)
			formattedArray[0] = oldArray[i];
	}
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function receives an array of strings and a reference of an mp3 tag.  It
 * goes through the arguments from a formatted array and puts them into the tag
 *
 * @param[in] arguments    - formatted array of arguments
 * @param[in, out] destTag - the tag to be modified
 *
 * @returns none
 *****************************************************************************/
void modifyTag(string arguments[15], mp3Tag &destTag)
{
	int i = 0;

	// Check if each field in a formatted argument array contains a flag
	// if it does, then put the value (in the i++ index) into the corrosponding
	// field in the destTag
	if(arguments[1].find("-s") != string::npos)
	{
		for(i = 0; i < 30; i++)
		{
			destTag.songTitle[i] = arguments[2][i];
		}
	}
	if(arguments[3].find("-a") != string::npos)
		for(i = 0; i < 30; i++)
		{
			destTag.artistName[i] = arguments[4][i];
		}
	if(arguments[5].find("-n") != string::npos)
		for(i = 0; i < 30; i++)
		{
			destTag.albumName[i] = arguments[6][i];
		}
	if(arguments[7].find("-c") != string::npos)
		for(i = 0; i < 28; i++)
		{
			destTag.comment[i] = arguments[8][i];
		}
	if(arguments[9].find("-y") != string::npos)
		for(i = 0; i < 4; i++)
		{
			destTag.year[i] = arguments[10][i];
		}
	if(arguments[11].find("-g") != string::npos)
		destTag.genre = extractIntFromString(arguments[12]);
	if(arguments[13].find("-t") != string::npos)
		destTag.track = extractIntFromString(arguments[14]);
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function is a subroutine to formatArgumentArray()  It does the actual
 * placement of arguemnts into their respective slots in the array
 *
 * @param[in] numberOfIterations  - iterator for the for loop
 * @param[in, out] formattedArray - the tag to be modified
 * @param[in] oldArray - the array passed into formatArgumentArray
 * @param[in] flagArray - array of booleans specifing which tags are to be organized
 *
 * @returns none
 *****************************************************************************/
void organizeArray(int numberOfIterations, string oldArray[20], 
							 string formattedArray[20], bool flagArray[8])
{
	int i, j;

	//Find each flag in the old array, then put the value corrosponding to that
	//flag in the correct spot in the formatted array
	for(j = 0; j < numberOfIterations; j++) {
		for(i = 1; i < 20; i++) {
			if (oldArray[i].find("-s") != string::npos)
	  		{
	  			formattedArray[1] = oldArray[i];
	  			formattedArray[2] = oldArray[i+1];
				flagArray[1] = true;
			}
			if (oldArray[i].find("-a") != string::npos)
			{
				formattedArray[3] = oldArray[i];
				formattedArray[4] = oldArray[i+1];
				flagArray[2] = true;
			}
			if (oldArray[i].find("-n") != string::npos)
			{
				formattedArray[5] = oldArray[i];
				formattedArray[6] = oldArray[i+1];
			flagArray[3] = true;
			}
			if (oldArray[i].find("-c") != string::npos)
			{
				formattedArray[7] = oldArray[i];
				formattedArray[8] = oldArray[i+1];
				flagArray[4] = true;
			}
			if (oldArray[i].find("-y") != string::npos)
			{
				formattedArray[9] = oldArray[i];
				formattedArray[10] = oldArray[i+1];
				flagArray[5] = true;
			}
			if (oldArray[i].find("-g") != string::npos)
			{
				formattedArray[11] = oldArray[i];
				formattedArray[12] = oldArray[i+1];
				flagArray[6] = true;
			}
			if (oldArray[i].find("-t") != string::npos)
			{
				formattedArray[13] = oldArray[i];
				formattedArray[14] = oldArray[i+1];
				flagArray[7] = true;
			}
		}
	}
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function prints out the genrelist
 *
 * @returns none
 *****************************************************************************/
void printGenrelist()
{
	int i;

	//Iterate through the list and print out each index
	for(i = 0; i < 148; i++)
	{
		cout << genrelist[i] << endl;
	}
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function prints out the usage statement
 *
 * @returns none
 *****************************************************************************/
void printUsageStatement() 
{
	cout << "\
Usage: prog1.exe [options] [Type] \n\
Options: -s value     change song title \n\
         -a value     change artist name \n\
         -n value     change album name \n\
         -c value     change comment \n\
         -t value     change track number \n\
         -y value     change year \n\
         -g value     change genre \n\
\
Type:    -f filename  change tags in mp3 file \n\
         -d directory change all mp3 files in directory \n\
                      -s, -t and -c not valid with -d type\n\
         -l genrelist list all genres with appropriate integers \n\
                      No other options may be given with this."  << endl;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function prints out the tag passed in
 *
 * @param[in] tag - the tag to be printed
 *
 * @returns none
 *****************************************************************************/
void printTag(mp3Tag &tag)
{
	cout << "  Title: " << tag.songTitle << endl;
	cout << " Artist: " << tag.artistName << endl;
	cout << "  Album: " << tag.albumName << endl;
	cout << "Comment: " << tag.comment << endl;
	cout << "  Track: " << tag.track << endl;
	cout << "   Year: " << tag.year << endl;
	cout << "  Genre: " << tag.genre << endl;
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function reads in the tag from the spefied file in the current directory
 *
 * @param[in] fin - an input file stream
 * @param[in] filename - the file to be read
 * @param[in, out] destTag - the tag in memory
 *
 * @returns none
 *****************************************************************************/
void readFile(ifstream &fin, string fileName, mp3Tag &destTag)
{
	//Open the file
	fin.open(fileName, ios::in | ios::binary);
	if(!fin)
	{
		cout << "File Failed to Open" << endl;
		exitWithError();
	}

	//Seek to the mp3tag at the end of the file, then read it in.
	fin.seekg(-128, ios::end);
	fin.read((char *) &destTag, sizeof(mp3Tag));

	cout << "--Original Tag--" << endl;

	//print the tag that was read in
	printTag(destTag);
	cout << endl;

	//close the file
	fin.clear();
	fin.close();
}

/**************************************************************************//**
 * @author Elliott Rarden
 *
 * @par Description:
 * This function writes a tag to the spefied mp3 file
 *
 * @param[in] fout - an output filestream
 * @param[in] filename - the file to be written to
 * @param[in] writetag - the tag to be written
 *
 * @returns none
 *****************************************************************************/
void writeFile(ofstream &fout, string filename, mp3Tag &writeTag)
{
	char check;

	//open the file and seek to the position of the tag
	fout.open(filename, ios::in | ios::out | ios::ate | ios::binary);
	fout.seekp(-128, ios::end);

	//output the modified tag
	cout << "--Modified Tag--" << endl;
	printTag(writeTag);
	cout << endl;

	//ask the user if they want to write the file, exit if thhey say no
	cout << "Are you sure you want to modify the file? (y/n)" << endl;
	cin >> check;
	switch (check)
	{
	case 'y':
		fout.write( (char *) &writeTag, sizeof( mp3Tag) );
		fout.clear();
		fout.close();
		break;

	case 'n':
		cout << "exiting" << endl;
		exit(0);
		break;

	default:
		cout << "Please enter y or n" << endl << "will now exit";
		exit(0);
		break;
	}	
}

