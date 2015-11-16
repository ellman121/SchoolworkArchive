#include <stdio.h>
#include <string.h>

#define MAX_DECIMAL_DIGITS 8

int base10double(char n1[MAX_DECIMAL_DIGITS])
{
  int i, tmp, carry=0;
  for(i = 0; i < 8; i++)
    {
      n1[i] += (n1[i] + carry);
      if(n1[i] > 9)
	{
	  n1[i] -= 10;
	  carry = 1;
	}
      else
	carry = 0;
    }
  return carry;
}

int strtoSfixed(char *s, int frac_bits)
{
  char *point = s;
  unsigned int value;
  int i, negative = 0;
  char digits[8];

  if(*s=='-')
    {
      negative = 1;
      s++;
    }
  value = atoi(s);
  /* find the decimal points */
  while((*point != '.') && (*point != 0))
    point ++;

  if(( *point == 0) || ( *(point +1) == 0))
    {
      if(negative)
	value = -value;
      return value << frac_bits;
    }
  ++point;

  for(i = (7); i > 0; i--)
    {
      if(*point == 0)
	digits[i] = 0;
      else
	{
	  digits[i] = *point - '0';
	  ++point;
	}
    }

  while(frac_bits > 0)
    {
      value <<= 1;
      if(base10double(digits))
	value |= 1;
      frac_bits--;
    }

  if(negative)
    value = -value;
  return value;
}

void printS(int num, int frac_bits)
{
  unsigned int mask = (1 << frac_bits) - 1;
  unsigned int fracpart;
  if(num < 0)
    {
      printf("-");
      num = -num;
    }

  printf("%d.",num>>frac_bits);

  fracpart = num & mask;

  do {
    fracpart *= 10;
    printf("%u", fracpart >> frac_bits);
    fracpart &= mask;
  } while (fracpart != 0);
}

double mydiv(double x, double y)
{
  x /= y;
  return x;
}
