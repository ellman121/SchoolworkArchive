//
//  main.cpp
//  DiffCrypto
//
//  Created by Elliott Rarden on 12/16/15.
//  Copyright © 2015 Elliott Rarden. All rights reserved.
//

#include <iostream>
#include <stdint.h>
#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const uint16_t S1[2][8] = {
	{ 5,2,1,6,3,4,7,0 },
	{ 1,4,6,2,0,7,5,3 }
};

const uint16_t S2[2][8] = {
	{ 4,0,6,5,7,1,3,2 },
	{ 5,3,0,7,6,2,1,4 }
};

const uint16_t COLA[16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

uint16_t COLB[16];

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

uint16_t lookupSB1(uint16_t bits)
{
	uint8_t row = (bits & 0xF) >> 3;
	uint8_t col = bits & 0x7;

	return S1[row][col];
}

uint16_t lookupSB2(uint16_t bits)
{
	uint8_t row = (bits & 0xF) >> 3;
	uint8_t col = bits & 0x7;

	return S2[row][col];
}

uint16_t prevKey(uint16_t currentkey)
{
	uint16_t temp = (currentkey & 0x1) << 8;

	currentkey = (currentkey >> 1);

	return temp | currentkey;
}

int main(int argc, char const *argv[])
{
	string str;
	uint16_t l1, r1, l1s, r1s; // Initial values (message and star)
	uint16_t l4, r4, l4s, r4s; // Encrypted values
	uint16_t l1p, r1p, l4p, r4p; // L'1, R'1, L'4, R'4
	uint16_t r4pl1p; // R'4 XOR L'1
	uint16_t s1in, s1out, s2in, s2out;

	vector<uint16_t> S1Matches, S2Matches;
	vector<uint16_t> LK4, RK4;
	vector<uint16_t> keys;
	S1Matches.resize(0);
	S2Matches.resize(0);
	LK4.resize(0);
	RK4.resize(0);

	/*
	 cout << "input L1, R1, L1*" << endl;
	 cin >> l1;
	 cin >> r1;
	 cin >> l1s;
	 l1s = l1; // R1 = R1Star

	 cout << "input L4, R4, L4*, R4*" << endl;
	 cin >> l4;
	 cin >> r4;
	 cin >> l4s;
	 cin >> r4s;
	 */

	// INPUTS HERE

	l1  = 28;
	r1  = 56;
	l1s = 25;
	l4  = 41;
	r4  = 45;
	l4s = 14;
	r4s = 28;

	//system("clear");

	// Step 3
	l4p = l4 ^ l4s;
	s1in = expand(l4p) >> 4; // Left half
	s2in = expand(l4p) & 15; // Right half

	cout << "Input to S1 = " << bitset<4>(s1in) << endl;
	cout << "Input to S2 = " << bitset<4>(s2in) << endl;
	cout << endl;

	// Step 4
	l1p = l1 ^ l1s;
	r4p = r4 ^ r4s;
	r4pl1p = r4p ^ l1p;
	s1out = r4pl1p >> 3; // Shift
	s2out = r4pl1p & 7; // Mask

	cout << "L1' = " << bitset<6>(l1p) << endl;
	cout << "R4' = " << bitset<6>(r4p) << endl;
	cout << "R4' XOR L1' = " << bitset<6>(r4pl1p) << endl;
	cout << "Output from S1 = " << bitset<3>(s1out) << endl;
	cout << "Output from S2 = " << bitset<3>(s2out) << endl;

	// Step 5 & 6
	// S-box 1
	for (int i = 0; i < 16; ++i)
	{
		COLB[i] = COLA[i] ^ s1in;
		if(s1out == (lookupSB1(COLA[i]) ^ lookupSB1(COLB[i])))
			S1Matches.push_back(i);
	}

	// S-box 2
	for (int i = 0; i < 16; ++i)
	{
		COLB[i] = COLA[i] ^ s2in;
		if(s2out == (lookupSB2(COLA[i]) ^ lookupSB2(COLB[i])))
			S2Matches.push_back(i);
	}

	cout << endl;
	cout << "S1Matches" << endl;
	for (int i = 0; i < S1Matches.size(); ++i)
	{
		cout << "\t" << bitset<4>(S1Matches[i]) << endl;
	}

	cout << "S2Matches" << endl;
	for (int i = 0; i < S2Matches.size(); ++i)
	{
		cout << "\t" << bitset<4>(S2Matches[i]) << endl;
	}

	// Step 7
	for (int i = 0; i < S1Matches.size(); ++i)
	{
		LK4.push_back((expand(l4) >> 4) ^ S1Matches[i]);
	}

	for (int i = 0; i < S2Matches.size(); ++i)
	{
		RK4.push_back((expand(l4) & 15) ^ S2Matches[i]);
	}

	// Output all possible keys
	int eo = 0;
	cout << endl << "KEYS: " << endl;
	for (int i = 0; i < LK4.size(); ++i)
	{
		for (int j = 0; j < RK4.size(); ++j)
		{
			uint16_t k = ((RK4[j] & 3) << 7) | ((eo ^ 1) << 6) | (LK4[i] << 2) | (RK4[j] >> 2);
			keys.push_back(k);
		}
		eo++;
		for (int j = 0; j < RK4.size(); ++j)
		{
			uint16_t k = ((RK4[j] & 3) << 7) | ((eo ^ 1) << 6) | (LK4[i] << 2) | (RK4[j] >> 2);
			keys.push_back(k);
		}
	}

	//sort(keys.begin(), keys.end());

	for (int i = 0; i < keys.size(); ++i)
	{
		cout << bitset<9>(keys[i]) << endl;
	}

	return 0;
}

/* TYLER INPUTS
	l1  = 36; // TEST VALUES
	r1  = 21;
	l1s = 29;
	r1s = 21;
	l4  = 16;
	r4  = 9;
	l4s = 6;
	r4s = 39;
 */

/* MY INPUTS
	l1  = 28;
	r1  = 56;
	l1s = 25;
	l4  = 41;
	r4  = 45;
	l4s = 14;
	r4s = 28;
 */

// CORRECT KEY:
// 110 011 110
