/* 
 *	Program: 		Hamming Code
 *	Author:  		Elliott Rarden
 *	Class:			CSC217 - Computer Org and Arch
 *	Instructor: 	Dr. Karlsson
 *	Compiltion:		Run makefile
 *	Usage:			[encode | parity | decode] <bitstring>
 *	
 */

 #include <iostream>
 #include <string>
 #include <cctype>
 #include <cstdlib>

using namespace std;

// List of valid commands, because it's easy to use and search through
string _VALID_COMMANDS[4] = {"encode", "parity", "decode"};

/*
 *		FUNCTION PROTOTYPES in the order they are implemented
 */
bool isValidCommand(string command);
int parseInput(string input, string &command, string &bitstring);
string decode74(string bitstring);
string decode1511(string bitstring);
string encode74(string bitstring);
string encode1511(string bitstring);
string vectoroutput(string bitstring);
void parity74(string bitstring);
void parity1511(string bitstring);
void promopt(bool full);

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		This is the main function of a program which does hamming
   encoding, parity checking, and decoding for 7-4 and 15-11 parity bit standards.
   To use, compile and run the executable.  The valid commands are 

   Parameters:
   		In  - string input
   		I/O - string command, string bitstring (they are expected to come in empty)
   		Out - True/False
*******************************************************************************/
int main(int argc, char const *argv[])
{
	bool fullPrompt = true;
	int valid = 0;
	string bitstring;
	string command;
	string input;

	// Loop for the program
	while(true)
	{
		// Output a promopt for the user
		promopt(fullPrompt);
		fullPrompt = false;

		// Clear our local strings for the next command
		bitstring.resize(0);
		command.resize(0);

		// Read in the user's input and figure out what they want us to do
		getline(cin, input);
		valid = parseInput(input, command, bitstring);

		// If the input isn't valid, discard it and go to the top
		if (valid != 0)
		{
			if (valid == 1)
			{
				cout << "Invalid input\n";
			}
			cout << "Please try again\n";
			fullPrompt = true;
			continue;
		}

		// Select our command and pass the right arguments to the functions
		if (command == "encode")
		{
			if (bitstring.size() == 4)
				cout
			<< "x = " << vectoroutput(encode74(bitstring)) << endl;
			else
				cout
			<< "x = " << vectoroutput(encode1511(bitstring)) << endl;
		}
		if (command == "decode")
		{
			if (bitstring.size() == 7)
				cout 
			<< "r = " << vectoroutput(decode74(bitstring)) << endl;
			else
 				cout 
 			<< "r = " << vectoroutput(decode1511(bitstring)) << endl;
		}
		if (command == "parity")
		{
			if (bitstring.size() == 7)
				parity74(bitstring);
			else
				parity1511(bitstring);
		}
	}

	return 0;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		

   Parameters:
   		In  - string command
   		Out - true/false
*******************************************************************************/
bool isValidCommand(string command)
{
	// This function is self-explanatory
	for (int i = 0; i <= 2; ++i)
	{
		if (command == _VALID_COMMANDS[i])
		{
			return true;
		}
	}
	return false;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Takes the input, and figures out what is the command
   and what is the bitstring.  It returns true or false, based on if the 
   input is valid or not

   Parameters:
   		In  - string input
   		I/O - string command, string bitstring (they are expected to come in empty)
   		Out - An integer depeing on the value of the function, see the table below

+---+-----------------------+
| 0 | Successful Parse      |
+---+-----------------------+
| 1 | Invalid Input length  |
+---+-----------------------+
| 2 | Invalid Command       |
+---+-----------------------+
| 3 | Invalid Bitstring     |
+---+-----------------------+
| 4 | Invalid Bitstring Len |
+---+-----------------------+
*******************************************************************************/
int parseInput(string input, string &command, string &bitstring)
{
	int pos = 0;
	int j;

	// Shortest valid command is less than 10 characters 
	if (input.size() <= 7 && input.size() >= 6)
	{
		return 1;
	}

	// Downcase everything for simplicity (only the first 7 characters are letters)
	for (int i = 0; i < input.size(); ++i)
	{
		if (isalpha(input[i]))
		{
			input[i] = tolower(input[i]);
		}
	}

	// Find the end of the first word
	while(!isspace(input[pos]))
	{
		pos++;
	}

	// Copy the string
	command = input.substr(0, pos);

	// Exit if we need to
	if (command == "exit")
		exit(0);

	// If the command isn't a valid command, return 2
	if (!isValidCommand(command))
	{
		cout << "Invalid command" << endl;
		return 2;
	}

	// Now a ll we have to do is get the bitstring
	bitstring = input.substr(pos+1);

	// If the bitstring isn't all 1's and 0's, it's not valid
	for (int i = 0; i < bitstring.size(); ++i)
	{
		if (bitstring[i] == '1' || bitstring[i] == '0')
		{ } else {
			cout << "Invalid Bitstring" << endl;
			return 3;
		}
	}

	int retval;
	// Make sure the bitstring is the correct length for the selected command
	if (command == "encode")
	{
		retval = ((bitstring.size() == 4) || (bitstring.size() == 11)) ? 0 : 4;
	} else
	if (command == "decode")
	{
		retval = ((bitstring.size() == 7) || (bitstring.size() == 15)) ? 0 : 4;
	} else
	if (command == "parity")
	{
		retval = ((bitstring.size() == 7) || (bitstring.size() == 15)) ? 0 : 4;
	}

	if (retval == 4)
	{
		cout << "Invalid bitstring length" << endl;
	}

	// We have a valid command and a valid bitstring.  We can safely return now
	return retval;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Decodes hamming74 on the given bitstring

   Parameters:
   		In  - string bitstring
   		Out - string
*******************************************************************************/
string decode74(string bitstring)
{
/*
+---+---+---+---+---+---+---+
| 0 | 0 | 1 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 1 | 0 | 0 |
+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 1 | 0 |
+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 1 |
+---+---+---+---+---+---+---+
*/
	string outstring;
	outstring.clear();
	outstring.resize(1);

	// It's an identity matrix, with 0 columns inserted to remove parity bits
	outstring[0] = bitstring[2];
	outstring.push_back(bitstring[4]);
	outstring.push_back(bitstring[5]);
	outstring.push_back(bitstring[6]);

	return outstring;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Decodes hamming 1511 on the given bitstring

   Parameters:
   		In  - string bitstring
   		Out - string
*******************************************************************************/
string decode1511(string bitstring)
{
/*
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
*/
	string outstring;
	outstring.clear();
	outstring.resize(11);

	// It's another identiy matrix, with 0 columns to remove parity bits
	outstring[0] = bitstring[2];
	outstring[1] = bitstring[4];
	outstring[2] = bitstring[5];
	outstring[3] = bitstring[6];
	outstring[4] = bitstring[8];
	outstring[5] = bitstring[9];
	outstring[6] = bitstring[10];
	outstring[7] = bitstring[11];
	outstring[8] = bitstring[12];
	outstring[9] = bitstring[13];
	outstring[10] = bitstring[14];

	return outstring;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Uses hamming74 encoding on the given bitstring

   Parameters:
   		In  - string bitstring
   		Out - string
*******************************************************************************/
string encode74(string bitstring)
{
/*
+---+---+---+---+
| 1 | 1 | 0 | 1 |
+---+---+---+---+
| 1 | 0 | 1 | 1 |
+---+---+---+---+
| 1 | 0 | 0 | 0 |
+---+---+---+---+
| 0 | 1 | 1 | 1 |
+---+---+---+---+
| 0 | 1 | 0 | 0 |
+---+---+---+---+
| 0 | 0 | 1 | 0 |
+---+---+---+---+
| 0 | 0 | 0 | 1 |
+---+---+---+---+
*/
	string outstring;
	outstring.clear();

	char a = bitstring[0]; // Get '1's and '0's from the bitstring
	char b = bitstring[1];
	char c = bitstring[2];
	char d = bitstring[3];

	int ia = a - '0'; // Turn them into ints
	int ib = b - '0';
	int ic = c - '0';
	int id = d - '0';

	// Since all of the hamming matricies are static, I just solved them
	// symbolically and this is the result
	outstring.append(to_string((ia + ib + id) % 2)); //P1
	outstring.append(to_string((ia + ic + id) % 2)); //P2
	outstring.append(to_string(ia)); //D1
	outstring.append(to_string((ib + ic + id) % 2)); //P3
	outstring.append(to_string(ib)); //D2
	outstring.append(to_string(ic)); //D3
	outstring.append(to_string(id)); //D4

	return outstring;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Uses hamming1511 encoding on the given bitstring

   Parameters:
   		In  - string bitstring
   		Out - string
*******************************************************************************/
string encode1511(string bitstring)
{
/*
+---+---+---+---+---+---+---+---+---+---+---+
| 1 | 1 | 0 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+
| 1 | 0 | 1 | 1 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 1 | 1 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 |
+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+
*/
	string outstring;
	outstring.clear();

	char a = bitstring[0]; // Some variables
	char b = bitstring[1];
	char c = bitstring[2];
	char d = bitstring[3];
	char e = bitstring[4];
	char f = bitstring[5];
	char g = bitstring[6];
	char h = bitstring[7];
	char i = bitstring[8];
	char j = bitstring[9];
	char k = bitstring[10];

	int ia = a - '0'; // Turn them into ints
	int ib = b - '0';
	int ic = c - '0';
	int id = d - '0';
	int ie = e - '0'; // Nobody likes Internet Explorer?  amIright?
	int inf = f - '0'; // Stupid if statements....ruining my nameing scheme
	int ig = g - '0';
	int ih = h - '0';
	int ii = i - '0';
	int ij = j - '0';
	int ik = k - '0';

	// Matrix math using general terms gives this resultant matrix
	outstring.append(to_string((ia + ib + id + ie + ig + ii + ik) %2)); //P1
	outstring.append(to_string((ia + ic + id + inf + ig + ij + ik) % 2)); //P2
	outstring.append(to_string(ia)); //D1
	outstring.append(to_string((ib+ ic + id + ih + ii + ij + ik) % 2)); //P3
	outstring.append(to_string(ib)); //D2
	outstring.append(to_string(ic)); //D3
	outstring.append(to_string(id)); //D4
	outstring.append(to_string((ie + inf + ig + ih + ii + ij + ik) % 2)); //P4
	outstring.append(to_string(ie)); //D5
	outstring.append(to_string(inf)); //D6
	outstring.append(to_string(ig)); //D7
	outstring.append(to_string(ih)); //D8
	outstring.append(to_string(ii)); //D9
	outstring.append(to_string(ij)); //D10
	outstring.append(to_string(ik)); //D11


	return outstring;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Returns the string in the following format
   			<X, X, X, ....., X>

   Parameters:
   		In  - string 
   		Out - string in the proper format
*******************************************************************************/
string vectoroutput(string bitstring)
{
    string outstring = "<"; // Some variables
	int loop = bitstring.size();
    int i = 0;

    // For each bit in the incomming bitstring, insert it with a ' ' and ','
	while(i < loop)
	{
	    outstring.push_back(' ');
	    outstring.push_back(bitstring[i]);
	    outstring.push_back(',');
	    i++;
	}

	// The string is currently "< X, X, X, ..., X,"
	outstring.erase(1,1); // <X, X, X, ..., X,
	outstring.erase(outstring.size() - 1,1); // <X, X, X, ..., X
	outstring.append(">"); // <X, X, X, ..., X>

	return outstring;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Checks the hamming74 parity for the given bitstring

   Parameters:
   		In  - string bitstring
*******************************************************************************/
void parity74(string bitstring)
{
/*
+---+---+---+---+---+---+---+
| 1 | 0 | 1 | 0 | 1 | 0 | 1 |
+---+---+---+---+---+---+---+
| 0 | 1 | 1 | 0 | 0 | 1 | 1 |
+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 1 | 1 | 1 | 1 |
+---+---+---+---+---+---+---+
*/
	bool bitstringIsGood = true; // I'm giving the user a lot of credit here
	string outstring;
	outstring.clear();

	char a = bitstring[0]; // Man, this looks familiar doesn't it?
	char b = bitstring[1];
	char c = bitstring[2];
	char d = bitstring[3];
	char e = bitstring[4];
	char f = bitstring[5];
	char g = bitstring[6];

	int ia = a - '0'; // I'd be shocked if you couldn't guess what I'm doing by now
	int ib = b - '0';
	int ic = c - '0';
	int id = d - '0';
	int ie = e - '0'; // Man, screw IE
	int inf = f - '0'; // I'm so mad about this
	int ig = g - '0';

	// More matrix math leads to this resultane matrix
	outstring.append(to_string((ia + ic + ie + ig) % 2));
	outstring.append(to_string((ib + ic + inf + ig) % 2));
	outstring.append(to_string((id + ie + inf + ig) % 2));

	// Output
	cout << "z = " << vectoroutput(outstring) << endl;
 
	// If Z isn't all 0's, fix X
	for (int i = 0; i < outstring.size(); ++i)
		if (outstring[i] != '0')
			bitstringIsGood = false;

	if(bitstringIsGood)
		return;

	// "Don't declare variables until they are needed" -- Socrates
	int fix_me = -1; // It starts at -1 because strings have index 0
	if (outstring[2] == '1')
		fix_me += 4;
	if (outstring[1] == '1')
		fix_me += 2;
	if (outstring[0] == '1')
		fix_me += 1;

	if (bitstring[fix_me] == '0')
		bitstring[fix_me] = '1';
	else
		bitstring[fix_me] = '0';

	cout << "x = " << vectoroutput(bitstring) << endl;
}

/*******************************************************************************
   Author:			Elliott Rarden
   
   Description:		Checks the hamming1511 parity for the given bitstring

   Parameters:
   		In  - string bitstring
*******************************************************************************/
void parity1511(string bitstring)
{
/* 
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 1 | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
*/
	bool bitstringIsGood = true; // I'm giving the user a lot of credit here
	string outstring;
	outstring.clear();

	char a = bitstring[0]; // Get '1's and '0's from the original bitstring
	char b = bitstring[1];
	char c = bitstring[2];
	char d = bitstring[3];
	char e = bitstring[4];
	char f = bitstring[5];
	char g = bitstring[6];
	char h = bitstring[7];
	char i = bitstring[8];
	char j = bitstring[9];
	char k = bitstring[10];
	char l = bitstring[11];
	char m = bitstring[12];
	char n = bitstring[13];
	char o = bitstring[14];

	int ia = a - '0'; // Convert them to integers
	int ib = b - '0';
	int ic = c - '0';
	int id = d - '0';
	int ie = e - '0'; // I already made the joke once, but, really, amIright?
	int inf = f - '0'; // I still hate if for this
	int ig = g - '0';
	int ih = h - '0';
	int ii = i - '0';
	int ij = j - '0';
	int ik = k - '0';
	int il = l - '0';
	int im = m - '0';
	int in = n - '0';
	int io = o - '0';

	// Do math
	outstring.append(to_string((ia + ic + ie + ig + ii + ik + im + io) % 2));
	outstring.append(to_string((ib + ic + inf + ig + ij + ik + in + io) % 2));
	outstring.append(to_string((id + ie + inf + ig + il + im + in + io) % 2));
	outstring.append(to_string((ih + ii + ij + ik + il + im + in + io) % 2));

	// Output our bitstring
	cout << "z = " << vectoroutput(outstring) << endl;

	// If we don't have all 0's in the parity check matrix, we need to fix it
	for (int i = 0; i < outstring.size(); ++i)
		if (outstring[i] != '0')
			bitstringIsGood = false;

	if(bitstringIsGood)
		return;

	// For memory saving purposes, I'm not declaring or initializing the int until I need it
	short fix_me = -1; // It starts at -1 because strings have index 0
	if (outstring[3] == '1')
		fix_me += 8;
	if (outstring[2] == '1')
		fix_me += 4;
	if (outstring[1] == '1')
		fix_me += 2;
	if (outstring[0] == '1')
		fix_me += 1;

	// Flip the bit
	if (bitstring[fix_me] == '0')
		bitstring[fix_me] = '1';
	else
		bitstring[fix_me] = '0';

	// More output
	cout << "x = " << vectoroutput(bitstring) << endl;
}

void promopt(bool full)
{	
	// Default prompt is 3 periods
	if (full == false)
	{
		cout << "\n> ";
		return;
	}
	// Full prompt
	else
	{
		cout << "\n Usage: {encode, decode, parity} <bitstring>\n \
The program will auto-detect 7-4 or 15-11\n \
'exit' will exit the program \n\n> ";
	}

}
