/*************************************************************************//**
 * @file 
 *
 * @brief This file contains the main function which modifies mp3 file tags
 *
 * @mainpage CSC250 Program 1
 * 
 * @section 11 AM - CSC250 - Computer Science 2 
 *
 * @author Elliott Rarden
 * 
 * @date March 2, 2014
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
 * @section Deatils and Usage
 * 
 * @details This is a program to modify mp3 file tags via the command line
 * 
 * @par Usage: 
   @verbatim  
   Usage: prog1.exe [options] [Type]
   Options: -s value     change song title
            -a value     change artist name
            -n value     change album name
            -c value     change comment
            -t value     change track number
            -y value     change year
            -g value     change genre
Type:       -f filename  change tags in mp3 file
            -d directory change all mp3 files in directory
                         -s, -t and -c not valid with -d typ
            -l genrelist list all genres with appropriate integers
                         No other options may be given with this.
 @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug I haven't tested on directories with more than 5 files, may crash
 * 
 * @par Modifications and Development Timeline: 
   @verbatim 
   Date          Modification 
   ------------  -------------------------------------------------------------- 
   Feb 10, 2014  Made the orignal function and project.  Built the genrelist
   Feb 17, 2014  Read in tags and output them to the command line.
   Feb 24, 2014  Modified and output the modified tags to the command line
   Feb 25, 2014  Wrote tags to the flie, and figured out directories
   Mar  2, 2014  Submitted the program for a program completion.
   @endverbatim
 *
 *****************************************************************************/
#include "functions.h"

int main(int argc, char **argv) 
{
	int numArguments;           // Number of command line argumetns passed
	int i, j;                   // Iterators
	string c_argv[20];          // Copy of argv
	ifstream fin;               // File input stream
	ofstream fout;              //File output stream 
	bool directoryMode = false; // Set true if "-d" is passed
	mp3Tag newTag = {};         // Tag holder
	string directory;           // The directory to change to

	/* The array Arguemtns is an array of c++ strings 
	 * 0 is the name of the file being modified
	 * All other entires are by default 0
	 * 1  is filled with "-s" and 2  is the new title
	 * 3  is filled with "-a" and 4  is the new artist name
	 * 5  is filled with "-n" and 6  is the new album name
	 * 7  is filled with "-c" and 8  is the new comment
	 * 9  is filled with "-y" and 10 is the new year
	 * 11 is filled with "-g" and 12 is the new genre
	 * 13 is filled with "-t" and 14 is the new track number
	 * 
	 * After formatting, all entries which do not contain changes will be set to the string 0
	 */
	string arguments[15];

	//Check for basic valid input.  Need a mininum of 3 arguemtns, no more than 17
	if (argc < 3 || argc > 17 )
	{
		cout << "Error: invalid input" << endl << endl;
		exitWithError();
	}
	
	//First things first - convert argv into an array of c++ strings
	for(i = 0; i < argc; i++)
    {
      c_argv[i] = argv[i];
	}
	//Resize every string in c_argv to 30 for easy math and parsing later. 
	for(i = 0; i < argc; i++)
	{
		c_argv[i].resize(30);
	}

	//Rename argc to numArguments.  Always good to avoid using argc/argv directly
	numArguments = argc;

	//Check for "-l genrelist."  
	//Will exit the program if any invalid input is found
	//otherwise will print the genrelist.
	for(i = 0; i < argc; i++)
	{
		if(c_argv[i].find("-l") != string::npos && c_argv[i+1].find("genrelist") != string::npos)
		{
			printGenrelist();
			return 0;
		}
	}

	// Parse the array and check that the command line arguments are valid
	for(i = 0; i < argc; i++)
	{
		//Check for valid use with -d.  If invalid use is found, exit
		if (c_argv[i].find("-d") != string::npos)
		{
			for (j = 0; j < numArguments; j++)
			{
				// Check for valid use
				// "-d" cannot be used with "-t", "-s", or "-c"
				if(c_argv[j].find("-t") != string::npos
				|| c_argv[j].find("-s") != string::npos
				|| c_argv[j].find("-c") != string::npos)
				{
					cout << endl << "invalid use of -d";
					exitWithError();
				}
			}

			// Set bool directoryMode to true
			directoryMode = true;

			// Store the name of the directory to change to for later use
			directory = c_argv[i+1];
		}

		//Check for valid use with -f.  If invalid use is found, exit
		if (c_argv[i].find("-f") != string::npos)
		{
			for (j = 0; j < numArguments; j++)
			{
				// Check for valid use
				// "-f" cannot be used with "-d", or "-l"
				if(c_argv[j].find("-d") != string::npos
				|| c_argv[j].find("-l") != string::npos)
				{
					cout << endl << "invalid use of -f";
					exitWithError();
				}
			}
		}
	}

	//check if directoryMode was set true.  If not, modify the single file
	if (directoryMode == false)
	{
		formatArgumentArray(c_argv, arguments, 17);
		changeFile(arguments, newTag);
	}

	//If directory mode is true, modify all the mp3 files in the directory
	if (directoryMode == true)
	{
		// Change to the directory specified.
		if (_chdir(directory.c_str()) != 0)
		{
			cout << "unable to change directories!" << endl; 
			exitWithError();
		}
		
		// Format the argument array just like we did for single files
		formatArgumentArray(c_argv, arguments, 17);

		//This block fills listOfFilesInDirectory with all the mp3
		//files in the directory, up to a maximum of 20.
		_finddata_t a_file;
		intptr_t  dir_handle;
		dir_handle = _findfirst( "*.mp3", &a_file);
		if( dir_handle == -1 ) {
			cout << "No mp3 files in the directory" << endl;
		}
		i = 0;

		//Assume there is at least 1 file in the directory, so we cna use a do
		// while loop
		do
		{
			if (i <= 10)
			{
				arguments[0] = a_file.name;
				i++;
			}

			//read in, modify, and write the tag from the file
			changeFile(arguments, newTag);
		}while( _findnext( dir_handle, &a_file ) == 0 ); //go to the next file
		_findclose( dir_handle ); //clsoe the directory
	}

	//Ask the user to press enter before closing the program
	cout << "Press enter to continue" << endl;
	cin.clear();
	cin.get();
	return 0;
}