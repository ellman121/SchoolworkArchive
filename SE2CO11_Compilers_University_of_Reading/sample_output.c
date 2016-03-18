#include <stdio.h>
int subfunc ( int var , int var2 ) {
return var - var2 ;
}
int main ( int argc , char const * argv [ ] ) {
int a ;
int b ;
printf("Input a number:");
scanf("%d", &a);
printf("Input another number:");
scanf("%d", &b);
int c = subfunc ( a , b ) ;
printf("%d - %d = %d\n", a, b, c);
return 0 ;
}
