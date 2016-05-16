#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>

// Partial compilation
#define USE_SEMAPHORES 1

#if USE_SEMAPHORES != 0
	// Initialise the semaphore
	sem_t mutex;
#endif

#define NUM_THREADS 8 /* define the total number of Threads we want */

/* Set global variable */
float total=0;

/* compute function just does something. */
void *compute()
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

	/* Critical Section - add POSIX semaphore */
	#if USE_SEMAPHORES != 0
		sem_wait(&mutex);
		total = oldtotal + result;
		sem_post(&mutex);
	#else
		total = oldtotal + result;
	#endif

	/* Print running total so far. */
	printf("Total is %f\n",total);

	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[NUM_THREADS];
	int i;
	int retcodes[NUM_THREADS];
	float result=0;

	printf("\n"); /* bit of whitespace */

	#if USE_SEMAPHORES != 0
		// Initialise the semaphore
		sem_init(&mutex, 0, 1);
	#endif
	

	/* Create the proper number of threads */
	for(i=0; i<NUM_THREADS; i++)
	{
		/* give a message about which thread we're creating */
		printf("Creating Thread #%d\n", i);

		/* Create the thread */
		retcodes[i] = pthread_create(&threads[i], NULL, compute, NULL);

		if (retcodes[i]) // If pthread_create gave us a value that isn't 0
		{
			printf("ERROR; return code from pthread_create() is %d\n", retcodes[i]);
			exit(-1); // Shit hit the fan
		}
	}

	pthread_exit(NULL);

	#if USE_SEMAPHORES != 0
		/* Once all threads are merged, we can destroy the semaphore */
		sem_destroy(&mutex);
	#endif
	

	/* nothing else to do so end main function (and program) */
	return 0;
}
