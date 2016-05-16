#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 8 /* define the total number of processes we want */

/* Set global variable */
float total=0;

/* compute function just does something. */
int compute()
{
	int i;
	float oldtotal=0, result=0;

	/* for a large number of times just square root and square
	the arbitrary number 1000 */
	for(i=2000000000;i!=0;i--)
	{
		result=sqrt(1000.0)*sqrt(1000.0);
	}

	/* Print the result – should be no surprise */
	printf("Result is %f\n",result);

	/* We want to keep a running total in the global variable total */
	oldtotal = total;
	total = oldtotal + result;

	/* Print running total so far. */
	printf("Total is %f\n",total);
	return(0);
}

int main()
{
	int pid[N], i, j;
	float result=0;

	printf("\n"); /* bit of whitespace */

	/* We want to loop to create the required number of processes
	Note carefully how only the child process is left to run */
	for(i=0;i<N;i++)
	{
		/* Do the fork and catch it if one fails */
		if((pid[i]=fork())==-1)
		{
			exit(1);
		}
		/* now with child we want to do our computation */
		else if(pid[i] > 0)
		{
			/* give a message about the proc ID */
			printf("Process Id for process %d is %d\n",i,getpid());

			/* call the function to do some computation. If we used sleep
			The process would simply sleep. We do not want that */
			compute();

			/* After we have done our computation we must quit the for
			loop otherwise we get a fork bomb! */
			break;
		}
	}

	sleep(5);

	/* nothing else to do so end main function (and program) */
	return 0;
}
