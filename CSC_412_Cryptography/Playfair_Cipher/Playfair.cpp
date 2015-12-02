#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Pointer is const, so you can't move the grid
char _GRID[5][5];

// Create the grid and store it in the global grid variable
void createGrid( string k )
{
	string uniqueKey = ""; // New key with only unique letters

	// If the provided key has no length, just use a default key
	if( k.length() < 1 )
	{
		k = "KEYWORD";
	}

	// Append the alphabet to k to gaurentee that all letters are in the grid
	k += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for( string::iterator si = k.begin(); si != k.end(); si++ )
	{
		*si = toupper( *si ); // Make everything uppercase
		if( *si < 65 || *si > 90 ) // If it's not a character, ignore it
			continue;

		if( *si == 'J' ) // Ignore J because it's the same as I
			continue;

		if( uniqueKey.find( *si ) == -1 ) // If the letter is not in the unique key
			uniqueKey += *si; // Append the letter
	}

	// Save it to the grid
	int pos = 0; // Current position in the key
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			_GRID[i][j] = uniqueKey[pos];
			pos++;
		}
	}
}

// Return the row of a given character.
int getRow(char x)
{
	if (!isalpha(x))
	{
		cout << "not a alpha character" << endl;
		return -1;
	}

	x = toupper(x);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (_GRID[i][j] == x)
			{
				return i;
			}
		}
	}

	return -1;
}

// Return the column of a given character.
int getCol(char x)
{
	if (!isalpha(x))
	{
		cout << "not a alpha character" << endl;
		return -1;
	}

	x = toupper(x);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (_GRID[i][j] == x)
			{
				return j;
			}
		}
	}

	return -1;
}

// Remove spaces and punctuation from p
string scrub(string in)
{
	string out = "";
	char temp = ' ';

	for (int i = 0; i < in.length(); ++i)
	{
		temp = in[i];
		if (isalpha(temp))
		{
			out += toupper(temp);
		}
	}

	return out;
}

int main(int argc, char const *argv[])
{
	bool encrypt;

	char first, second;
	int FR, FC, SR, SC; // First/Second Row/Col
	int pos = 0;

	string k = "playfair";
	string in = "meet at the schoolhouse";
	string out = "";

	if (argc != 4)
	{
		cout << "Usage: ./playfair -{e,d} <key> <message>" << endl;
		return 1;
	}

	if (argv[1][1] == 'e' || argv[1][1] == 'd')
	{
		encrypt = (argv[1][1] == 'e');
	}
	else
	{
		cout << "Must use -e or -d as the first argument" << endl;
		cout << "-e for encryption mode" << endl;
		cout << "-d for decryption mode" << endl;
	}

	k = string(argv[2]);
	in = string(argv[3]);

	// Make the grid
	createGrid(k);

	// Output the grid for good measure
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			cout << _GRID[i][j] << " ";
		}
		cout << endl;
	}

	// Remove spaces and punctuation from p
	in = scrub(in);

	// Get characters 2 at a time and encrypt them
	while(pos < in.length())
	{
		first = in[pos];
		pos++;
		if (pos == in.length())
		{
			second = 'X';
		}
		else
		{
			second = in[pos];
			if (first == second)
			{
				second = 'X';
			}
			else
			{
				pos++;
			}
		}

		// Get the row and column for the two characters
		FR = getRow(first);
		FC = getCol(first);
		SR = getRow(second);
		SC = getCol(second);

		// If same row, rotate right and continue
		if(FR == SR)
		{
			if (encrypt)
			{
				out += _GRID[FR][(FC + 1) % 5];
				out += _GRID[SR][(SC + 1) % 5];
			}
			else
			{
				out += _GRID[FR][(FC + 4) % 5];
				out += _GRID[SR][(SC + 4) % 5];
			}
			continue;
		}

		// If same column, rotate down and continue
		if (FC == SC)
		{
			if (encrypt)
			{
				out += _GRID[(FR + 1) % 5][FC];
				out += _GRID[(SR + 1) % 5][SC];
				continue;
			}
			else
			{
				out += _GRID[(FR + 4) % 5][FC];
				out += _GRID[(SR + 4) % 5][SC];
				continue;
			}
		}

		// Rectange case - same row opposite corner
		out += _GRID[FR][SC];
		out += _GRID[SR][FC];
	}

	// output
	pos = 0;
	while(pos < out.length())
	{
		first = out[pos];
		pos++;
		second = out[pos];
		pos++;

		cout << first << second << " ";
	}
	cout << endl;

	return 0;
}
