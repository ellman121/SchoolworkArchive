#include <stdio.h>

int subfunc(int var, int var2)
{
	return var - var2;
}

int main(int argc, char const *argv[])
{
	int a;
	int b;
	cout << "Input a number: ";
	cin >> a;
	cout << "Input another number: ";
	cin >> b;
	int c = subfunc(a, b);
	cout << a << " - " << b << " = " << c << endl;
	return 0;
}
