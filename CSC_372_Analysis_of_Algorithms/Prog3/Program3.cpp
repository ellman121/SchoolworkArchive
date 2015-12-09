// Program1.cpp
// Elliott Rarden
// First assignment for Analysis of Algorithms
// 
// Usage:
// 		1) compile with g++
// 		2) ./a.out <inputfile>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <complex>
#include <cmath>
#include <iomanip>

using namespace std;

// I is a constant
const complex<double> I (0, 1);

////////////////////////////////////////////////////////////////////
//                        FFT based on CLRS page 911              //
//                        Borrowed from the Analysis			  //
//                           of Algorithms Website  			  //
////////////////////////////////////////////////////////////////////
void fft(complex <double> a[], const int n, complex <double> y[])
{
	complex<double> *even = new complex<double>[n];
	complex<double> *even_fft = new complex<double>[n];
	int 			i;
	int 			j;
	int 			n2;
	complex<double> *odd = new complex<double>[n];
	complex<double> *odd_fft = new complex<double>[n];
	complex<double> omega;
	complex<double> omega_power;

	if (n == 1)
	{
		y[0] = a[0];

		delete[] even;
		delete[] even_fft;
		delete[] odd;
		delete[] odd_fft;

		return;
	}

	n2 = n / 2;
	j = 0;
	for (i = 0; i < n; i += 2)
	{
		even[j] = a[i];
		odd[j] = a[i + 1];
		j ++;
	}

	fft(even, n2, even_fft);
	fft(odd, n2, odd_fft);

	omega = cos(-2.0 * M_PI / n) + I * sin(-2.0 * M_PI / n);

	omega_power = 1;
	for (i = 0; i < n2; i ++)
	{
		y[i] = even_fft[i] + omega_power * odd_fft[i];
		y[i + n2] = even_fft[i] - omega_power * odd_fft[i];
		omega_power = omega * omega_power;
	}

	delete[] even;
	delete[] even_fft;
	delete[] odd;
	delete[] odd_fft;
}

/* int main()
 *
 * Author: Elliott Rarden
 *
 * Description: This function reads in values from the file, calls Dr. Corwin's
 * FFT and then does iterates the remaining transforms
 *
 * Parameters:
 * 		Input:		argc - Argument count
 * 					argv - Argument values
 * 		Returns:	An integer denoting any errors (0 for no errors)
 */
int main(int argc, char const *argv[])
{
	complex<double> *y; 	// y array of complex values
	complex<double> *a; 	// Initial values from the file (coefficients)
	complex<double> omega; 	// Omega
	complex<double> x; 		// x for division later
	double *total, temp;	// Array to keep the totals.  Also a temp holder
	int n, k, i, indx;		// n, k, loop variable, and an index keeper
	string file = argv[1];	// The file comes in from argv[1]

	// File handlers
	ifstream fin;
	ofstream fout;

	// Open the files
	fin.open(file);
	fout.open(file.substr(0,file.rfind('.')) + ".out");

	// Read in n and k
	fin >> n;
	fin >> k;

	// Get some arrays
	a = new complex<double>[n];
	y = new complex<double>[n];
	total = new double[n];

	// Read in n
	for (i = 0; i < n; ++i)
	{
		fin >> temp;
		a[i] = temp;
	}

	// Call the FFT
	fft(a, n, y);

	// Assign our total array som initial values from the first fft
	for (i = 0; i < n; ++i)
	{
		total[i] = abs(y[i]);
	}

	// All of the FFT's
	for (i = 0; i < k-1; ++i)
	{
		// x starts at one, omega starts at e^(2 * pi / n)
		x = 1;
		omega = cos(2.0 * M_PI / n) + I * sin(2.0 * M_PI / n);

		// Read in our next value from the array
		fin >> temp;

		// Cast temp to a complex, and 0 out the imaginary part
		complex<double> t(temp,0);

		// Run the formula n times
		for (int j = 0; j < n; ++j)
		{
			y[j] = (y[j] - a[i] + t) / x;
			x *= omega;
			total[j] += abs(y[j]); // Running total
		}
	}

	// Average things over the number of FFT's we did
	for (i = 0; i < n; ++i)
	{
		total[i] /= k;
	}

	// Output top 5 values
	for (i = 0; i < 5; ++i)
	{
		temp = 0;

		// Find the peak
		for (int j = 1; j < n/2; ++j)
		{
			if (temp < total[j]) // If we have a peak
			{
				temp = total[j];
				indx = j;
			}
		}

		// Round off the peak
		total[indx] += total[indx-2] + total[indx-1] + total[indx+1] + total[indx+2];

		// Output the peak
		fout << indx << " " << fixed << setprecision(2) << total[indx] << endl;

		// Set this peak to 0 so we don't find it again
		total[indx] = 0;
	}

	// Close file handlers
	fin.close();
	fout.close();

	// Delete dynamically allocated arrays
	delete[] a;
	delete[] y;
	delete[] total;

	// We're done
	return 0;
}
