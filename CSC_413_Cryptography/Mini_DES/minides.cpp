/*//////////////////////////////////////////////////////////////////////////////
  Program:
    Mini-DES encrypt/decrypt

  Author:
    Tyler Henson & Elliott Rarden

  Date:
    Oct 5 2015

  Instructor(s): Dr. Brent Deschamp

  Course:
    Cryptography

  Description:
    The perpose of this program is to implement a simplified version of the
  DES encryption method. We take in a key of 9 bits with a message of 12 bits
  and we encrypt 6 bits at a time. Real DES has much larger sets of bits
  encrypted at a time, and as a result has larger keys. This program can do
  any number of rounds of encryption but it is set to 4 rounds.
  
    Details of this algorithm start on page 114 of Introduction to Cryptography
  by Wade Trappe, Lawrence C. Washington

  Usage:
	Encryption:
		./minides -e <key> <message>
	Decryption:
		./minides -d <key> <message>

  Bug/todo:
    none

  Timeline:

  Date			      Modification
--------------  ----------------------------------------------------------------
  Oct 3           Started program
  Oct 4           rewriting program & fixing bugs
  Oct 5           Finished program

//////////////////////////////////////////////////////////////////////////////*/
#include <iostream>
#include <cstdint>
#include <bitset>

using namespace std;

// Global SBoxes
uint16_t sb1[2][8] =
{
	{5, 2, 1, 6, 3, 4, 7, 0},
	{1, 4, 6, 2, 0, 7, 5, 3}
};
char sb2[2][8] =
{
	{4, 0, 6, 5, 7, 1, 3, 2},
	{5, 3, 0, 7, 6, 2, 1, 4}
};

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    When this finds a 1 in the string, it adds that binary representation to
  the integer ret. 

  Inputs:
    s- a string of bits (e.g. "1 0010 1101")

  Outputs:
    ret- the integer representation of s (e.g. 301)
//////////////////////////////////////////////////////////////////////////////*/
uint16_t bits_to_int(string s)
{
	int length = s.size();
	int radix = 1 << (length - 1);
	uint16_t ret = 0;

	for (int i = 0; i < length; i++)
	{
		if(s.at(i) == '1')
		{
			ret |= radix >> i;
		}
	}

	return ret;
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    This function is nessisary to find the key order for encryption
  For example, if you input the first key k1, it will return the second key, k2

  Inputs:
    currentkey - k(i)

  Outputs:
    - k(i+1) a.k.a. the next key
//////////////////////////////////////////////////////////////////////////////*/
uint16_t nextKey(uint16_t currentkey)
{
	uint16_t temp = (currentkey & 0x100) >> 8;

	currentkey = (currentkey << 1);

	return temp | currentkey;
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    This function is nessisary to find the reverse key order for decryption
  For example, if you input the last key k4, it will return the second to last
  key, k3

  Inputs:
    currentkey - k(i)

  Outputs:
    - k(i-1) a.k.a. the previous key
//////////////////////////////////////////////////////////////////////////////*/
uint16_t prevKey(uint16_t currentkey)
{
	uint16_t temp = (currentkey & 0x1) << 8;

	currentkey = (currentkey >> 1);

	return temp | currentkey;
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    All this does is expand 6 bits to 8 bits in the way specified by DES.

  Inputs:
    input-lowest 6 bits of block of 12 bits

  Outputs:
    temp - expanded input to 8 bits
//////////////////////////////////////////////////////////////////////////////*/
uint16_t expand(uint16_t input)
{
	uint16_t temp = input & 0x03;
	temp |= (input & 0x30) << 2;

	// Put bit 3 (the fourth one - book is not 0 indexed) in spot 3 and 5
	uint16_t bit = input & 0x08;
	temp |= bit >> 1;
	temp |= bit << 1;

	// Now put bit 2 in place
	bit = input & 0x04;
	temp |= bit << 1;
	temp |= bit << 3;

	return temp;
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    Scrambles bits by puting it through sbox 1

  Inputs:
    bits- 4 bits that represent which row and column should be accessed from sbox

  Outputs:
    sb2 - integer in a specific spot from s box 1
//////////////////////////////////////////////////////////////////////////////*/
uint16_t lookupSB1(uint16_t bits)
{
	uint8_t row = (bits & 0xF) >> 3;
	uint8_t col = bits & 0x7;

	return sb1[row][col];
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    Scrambles bits by puting it through sbox 2

  Inputs:
    bits- 4 bits that represent which row and column should be accessed from sbox

  Outputs:
    sb2 - integer in a specific spot from s box 2
//////////////////////////////////////////////////////////////////////////////*/
uint16_t lookupSB2(uint16_t bits)
{
	uint8_t row = (bits & 0xF) >> 3;
	uint8_t col = bits & 0x7;

	return sb2[row][col];
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    This function expands the message xor's that with the key, and saves the
  new message as an integer.

  Inputs:
    key- 8 bit key
    message- 6 bit message
  Outputs:
    -encrypted or decryted message
//////////////////////////////////////////////////////////////////////////////*/
uint16_t func(uint16_t key, uint16_t message)
{
	uint16_t expanded = expand(message);

	uint16_t xored = key ^ expanded;

	// Left is lower bits (book is backwards)
	uint16_t right = xored & 0xF;
	uint16_t left = (xored & 0xF0) >> 4;

	left = lookupSB1(left);
	right = lookupSB2(right);

	return (left << 3) | right;
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    This applies the DES encryption method. In short, expand 6 bits or R1 to
  8 bits, XOR that with Ki (which is 8 bits of K, by rotation), split the string
  in half. Put the first 4 bits in sbox 1 and the next 4 bits in sbox 2, 
  recombine the bits, and pass R1 down as L2 and the recombined bits as R2.

  Inputs:
    key - 9 bit key
    message - the entire input to encrypt
    rounds - number of times we will encrypt (in this case 4)

  Outputs:
    -decrypted integer
//////////////////////////////////////////////////////////////////////////////*/
uint16_t encrypt(uint16_t key, uint16_t message, uint16_t rounds)
{
	uint16_t leftM; // Left of message
	uint16_t rightM; // Right of message
	uint16_t k = key; // Local copy of key
	uint16_t nextLeft;
	uint16_t nextRight;

	leftM = (message & 0xFC0) >> 6;//set L1 & R1
	rightM = message & 0x3F;

	while(rounds > 0)// apply 4 rounds of DES encryption
	{
		nextLeft = rightM;
		rightM = func(k >> 1, rightM);
		nextRight = rightM ^ leftM;
		rightM = nextRight;
		leftM = nextLeft;
		k = nextKey(k);
		rounds--;
	}

	return (leftM << 6) | rightM;

}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    The only notable difference between this and encrypt is that the keys are
  applied in reverse order. and that the pair of 6 bits are switched at the 
  start of each round.

  Inputs:
    key - 9 bit key
    message - the entire input to decrypt
    rounds - number of times we will decrypt (in this case 4)

  Outputs:
    -decrypted integer
//////////////////////////////////////////////////////////////////////////////*/
uint16_t decrypt(uint16_t key, uint16_t message, uint16_t rounds)
{
	uint16_t leftM; // Left of message
	uint16_t rightM; // Right of message
	uint16_t k = key; // Local copy of key
	uint16_t nextLeft;
	uint16_t nextRight;

	rightM = (message & 0xFC0) >> 6;//Set R4 and L4
	leftM = message & 0x3F;

	for (int i = 1; i < rounds; ++i)//find k4
	{
		k = nextKey(k);
	}

	while(rounds > 0)//apply DES decryption rounds times
	{
		nextLeft = rightM;
		rightM = func(k >> 1, rightM);
		nextRight = rightM ^ leftM;
		rightM = nextRight;
		leftM = nextLeft;
		k = prevKey(k);
		rounds--;
	}

	return (leftM) | (rightM << 6);
}

/*//////////////////////////////////////////////////////////////////////////////
  Author:
    Elliott Rarden & Tyler Henson

  Description:
    Calls either encrypt or decrypt depending on which code is used. Either e
  or d respectively. This function also gets the key and message from the user.

  Inputs:
    argc- number of inputs
    argv[]- array of inputs

  Outputs:
    1- error
    0- program worked correctly
//////////////////////////////////////////////////////////////////////////////*/
int main(int argc, char const *argv[])
{
	char mode = argv[1][1];
	string key = argv[2];
	string message = argv[3];
	
	uint16_t k = bits_to_int(key);
	uint16_t m = bits_to_int(message);

	uint16_t rounds = 4;

	if (mode != 'd' && mode != 'e')//error check
	{
		cout << "Invalid mode" << endl;
		return 1;
	}

	cout << ((mode == 'e') ? "Encryption" : "Decryption") << " Mode" << endl;

	if (mode == 'e')//encrypt string
	{ 
		uint16_t enc = encrypt(k, m, rounds);
		cout << bitset<6>(enc >> 6) << " " << bitset<6>(enc) << endl;
	}
	else
	{
		uint16_t dec = decrypt(k, m, rounds);//decrypt string
		cout << bitset<6>(dec >> 6) << " " << bitset<6>(dec) << endl;
	}

	return 0;
}
