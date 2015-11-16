/*************************************************************************//**
 * Assignment 3 - List similarities x3
 *
 * Elliott Rarden
 *
 * December 2, 2014
 * Professor: Dr. E. Corwin
 * Course: CSC 300 - 10:00am
 * Location: McLaurey 205
 *
 * Program Information
 *
 * Details
 *   This program reads in two lists of 32-bit integers (seperated by a negative
 * value) from a text file called "join.in"  It will then go through the list
 * and find the number of similar numbers (numbers in both lists) three ways.
 *
 *    First way  - A simple nested loop that goes through both lists int by int
 *    Second way - Sort the lists and go through similar to a merge
 *    Third way  - Hash the first list and count colissions on the second.
 *
 * Compiling Instructions:
 *   Use g++ to compile, uses C++11 Functions
 *****************************************************************************/
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

/*******************************************************************************
 *                          Global Constants
 ******************************************************************************/
const int HASH_TABLE_NULL = -1;

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
float clock_seconds();
void hash_insert(int x, int *&hash_table, int tab_size);
bool hash_find(int x, int *hash_table, int tab_size);
float opt_one(vector<int> x, vector<int> y, int &num);
float opt_two(vector<int> x, vector<int> y, int &num);
float opt_three(vector<int> x, vector<int> y, int &num);

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 * This is the main function of a program to find similarities in lists
 *
 * Paramaters:
 *   IN  - Number of command-line arguments
 *       - 2D Char Array containing argument values
 *
 * Returns:
 *   0 if no errors were encountered
==============================================================================*/
int main(int argc, char** argv)
{
	float first_time;
	float second_time;
	float third_time;

	int temp;

	ifstream fin;
	ofstream fout_value;
	ofstream fout_time;

	vector<int> one;
	vector<int> two;

	fin.open("join.in");
	fout_time.open("times.out");
	fout_value.open("values.out");

	// Simple loop to read in the first list - break when it finds a negative number
	while(fin >> temp)
	{
		if (temp < 0)
		{
			break;
		}
		one.push_back(temp);
	}

	// Simple loop to read in the second
	while(fin >> temp)
	{
		two.push_back(temp);
	}

	// FIRST OPTION - NESTED LOOP
	first_time = opt_one(one, two, temp);
	if (first_time == -1)
	{
		fout_time << "Option 1 time: incomplete" << endl;
		fout_value << "Option 1 result: incomplete" << endl;
	}
	else
	{
		// Fix roundoff error for floating point numbers
		if (first_time < 0.0001)
			first_time = 0;
		fout_time << fixed << "Option 1 time: " << setprecision(4) << first_time << endl;
		fout_value << fixed << "Option 1 result: " << temp << endl;
	}

	second_time = opt_two(one, two, temp);
	if (second_time == -1)
	{
		fout_time << "Option 2 tme: incomplete" << endl;
		fout_value << "Option 2 result: incomplete" << endl;
	}
	else
	{
		// Fix roundoff error for floating point numbers
		if (second_time < 0.0001)
			second_time = 0;
		fout_time << fixed << "Option 2 time: " << setprecision(4) << second_time << endl;
		fout_value << fixed << "Option 2 result: " << temp << endl;
	}

	third_time = opt_three(one, two, temp);
	if (third_time == -1)
	{
		fout_time << "Option 3 tme: incomplete" << endl;
		fout_value << "Option 3 result: incomplete" << endl;
	}
	else
	{
		// Fix roundoff error for floating point numbers
		if (third_time < 0.0001)
			third_time = 0;
		fout_time << fixed << "Option 3 time: " << setprecision(4) << third_time << endl;
		fout_value << fixed << "Option 3 result: " << temp << endl;
	}

	return 0;
}

/* =============================================================================
 * Author: Dr. E. Corwin
 *
 * Description:
 *   Returns the current time as a float
 *
 * Returns:
 *   The current time as a float
==============================================================================*/
float clock_seconds()
{
	return clock() / (float) CLOCKS_PER_SEC;
}

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 *   This function hashes x into the hash table
 *
 * Paramaters:
 *   IN  x - The integer to be hashed into the table
 *       hash_table - The destinaiton for hash(x)
 *       tab_size - The size of the table
==============================================================================*/
void hash_insert(int x, int *&hash_table, int tab_size)
{
	int i = 0;
	int hash_key = x % tab_size;
	while(i < tab_size)
	{
		if (hash_table[hash_key] == HASH_TABLE_NULL)
		{
			hash_table[hash_key] = x;
			return;
		}
		i++;
		hash_key = (x + i*i) % tab_size;
	}
}

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 *   This function looks for x in the table
 *
 * Paramaters:
 *   IN  x - The integer to be found
 *       hash_table - The hash table to look through
 *       tab_size - The size of the table
 *
 * Returns:
 *   T/F if the number was found
==============================================================================*/
bool hash_find(int x, int *hash_table, int tab_size)
{
	int i = 0;
	int hash_key = x % tab_size;
	while(i < tab_size)
	{
		if(hash_table[hash_key] == HASH_TABLE_NULL)
		{
			return false;
		}
		if(hash_table[hash_key] == x)
		{
			return true;
		}
		i++;
		hash_key = (x + i*i) % tab_size;
	}
	return false;
}

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 *   This function reads through the first list and check every number against
 * the entire second list.  This algorithm runs in n^2
 *
 * Paramaters:
 *   IN  x - First list
 *       y - Second list
 *   I/O num - The number of shared integers
 *
 * Returns:
 *   A floating point number for the processor time required
==============================================================================*/
float opt_one(vector<int> x, vector<int> y, int &num)
{
	float current_time;
	float start_time = clock_seconds();
	float ret_time;

	num = 0;

	current_time = clock_seconds();

	// Loop through one vector
	for (int i = 0; i < x.size(); i++)
	{
		// Loop through the next vector
		for (int j = 0; j < y.size(); j++)
		{
			//If number is in common, increment count
			if(x[i] == y[j])
			{
				num++;
			}
		}

		// Check the current time
		current_time = clock_seconds();
		if(current_time - start_time > 30)
		{
			break;
		}
	}

	current_time = clock_seconds();
	ret_time = current_time - start_time;
	if (ret_time > 30)
		ret_time = -1;

	return ret_time;
}


/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 *   This function first sorts the two lists and then walks simultaneously
 * walks through to find shared numbers.  This is O(n*lg(n)) based on the time
 * required to sort the integers.
 *
 * Paramaters:
 *   IN  x - First list
 *       y - Second list
 *   I/O num - The number of shared integers
 *
 * Returns:
 *   A floating point number for the processor time required
==============================================================================*/
 float opt_two(vector<int> x, vector<int> y, int &num)
 {
 	float current_time;
	float start_time = clock_seconds();
	float ret_time;

	int i = 0;  // Iterators
	int j = 0;
	int x_s = x.size();
	int y_s = y.size();
	num = 0;    // Reset num to 0

	// Copy both lists
	vector<int> a = x;
	vector<int> b = y;

	// Sort both lists using STL Sort ( O(n*lg(n)) )
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	// A simple modification of the merge operation from mergesort
	while(i < x_s && j < y_s)
	{
		// If a[i] is bigger than b[j], increment j
		if(a[i] > b[j])
		{
			j++;
			continue;
		}
		// If they are equal, incremnet.  We can increment both i and j because
		// we are gaurenteed unique key values
		if (a[i] == b[j])
		{
			num++;
			i++;
			j++;
			continue;
		}
		// If a[i] is less than b[j], increment i
		if(a[i] < b[j])
		{
			i++;
			continue;
		}
	}

	// Get the current time -- This still works even if we broke from the loop
	current_time = clock_seconds();
	ret_time = current_time - start_time;
	if (ret_time > 30)
		ret_time = -1;

	return ret_time;
 }

/* =============================================================================
 * Author: Elliott Rarden
 *
 * Description:
 *   This function hashes the first vector into an array, using quadratic
 * probing and then searches the array for elements in the second vector
 *
 * Paramaters:
 *   IN  x - First list
 *       y - Second list
 *   I/O num - The number of shared integers
 *
 * Returns:
 *   A floating point number for the processor time required
==============================================================================*/
float opt_three(vector<int> x, vector<int> y, int &num)
{
	// Some variables to keep time with
 	float current_time;
	float start_time = clock_seconds();
	float ret_time;

	// The number of shared values
	num = 0;

	// Table size is 2x the size of the array, so we don't overflow
	int tab_size = x.size() * 2;
	int *hash_table = nullptr;
	hash_table = new int[tab_size];
	if (hash_table == nullptr)
	{
		cerr << "Failed to allocate hash_table" << endl;
		return -1;
	}

	// Clear out the hash_table
	for (int i = 0; i < tab_size; i++)
	{
		hash_table[i] = HASH_TABLE_NULL;
	}

	// Insert each value in x into the hash table
	for (std::vector<int>::iterator i = x.begin(); i != x.end(); i++)
	{
		hash_insert(*i, hash_table, tab_size);
	}

	for (std::vector<int>::iterator i = y.begin(); i != y.end(); ++i)
	{
		if(hash_find(*i, hash_table, tab_size))
			num++;
	}

	// Get the current time
	current_time = clock_seconds();
	ret_time = current_time - start_time;
	if (ret_time > 30)
		ret_time = -1;

	// Only you can prevent forrest fires....I mean memory leaks....
	delete[] hash_table;

	return ret_time;
}
