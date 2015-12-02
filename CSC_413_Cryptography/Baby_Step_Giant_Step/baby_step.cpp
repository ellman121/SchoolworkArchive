#include <cmath>
#include <stdint.h>
#include <iostream>

using namespace std;

// Fast exponent code.  Straight from Analysis of Algorithms.
uint64_t powermod(uint64_t base, uint64_t exponent, uint64_t mod)
{
	if(exponent == 0)
		return 1;
	if(exponent == 1)
		return base;

	if (exponent % 2 == 1)
		return base * powermod(base, exponent-1, mod) % mod;

	uint64_t temp = powermod(base, exponent >> 1, mod);
	return (temp * temp) % mod;
}

// Return the multiplicative inverse of a mod p
// borrowed from RosettaCode
int mul_inv(int a, int p)
{
	int p0 = p, t, q;
	int x0 = 0, x1 = 1;

	// If the prime is one, there is no inverse
	if (p == 1)
		return 1;

	// Optimized euclidian algorithm.  I'm just ignore what I don't need
	while (a > 1)
	{
		q = a / p; // Quotient
		t = p, p = a % p, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}

	if (x1 < 0)
		x1 += p0;

	return x1;
}

// if a^x % p = b, return true
bool check(uint64_t a, uint64_t x, uint64_t b, uint64_t p)
{
	return powermod(a, x, p) == b
}

// Read in the variables, build the tables, search the tables, output
// Pretty straightforward stuff
int main(int argc, char const *argv[])
{
	bool sign;
	uint64_t a, b, p, n, a_inv, temp, x;
	uint64_t *arrayone, *arraytwo;

	// Read in the values from the file.
	cin >> p;
	cin >> a;
	cin >> b;

	// We need to have exactly n^2 > p, which is always gaurenteed
	// with n = sqrt(p-1) + 1
	n = sqrt(p-1) + 1;
	arrayone = new uint64_t[n];
	arraytwo = new uint64_t[n];

	// Generate the first list with a^j (mod p)
	for (int j = 0; j < n; ++j)
	{
		arrayone[j] = powermod(a, j, p);
	}

	// Generate the second list using b * a^((-1) * n * k) (mod p)
	// We calculate a^(-1) first, because a^((-1) * n * k) = (a^(-1))^(n*k)
	// and a^(-1) is constant
	a_inv = mul_inv(a, p);
	for (int k = 0; k < n; ++k)
	{
		arraytwo[k] = (b * powermod(a_inv, n * k, p)) % p;
	}

	// O(n^2) to walk through the lists.  I'm not sure if there's a better way
	// to search since we need the indeces as well as the values.
	for (uint64_t j = 0; j < n; ++j)
	{
		for (uint64_t k = 0; k < n; ++k)
		{
			if (arrayone[j] == arraytwo[k])
			{
				x = (j + n*k) % p;
				if (check(a, x, b, p))
				{
					cout << j << " " << n << " " << k << " " << x << endl;
				}
			}
		}
	}

	return 0;
}
