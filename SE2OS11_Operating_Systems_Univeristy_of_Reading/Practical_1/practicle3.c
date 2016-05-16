#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	// index 0 = read
	// index 1 = write
	int pipe1[2];
	pipe(pipe1);

	int pipe2[2];
	pipe(pipe2);

	if (!fork())
	{ 	// Process 0

		// There is no input from pipe1
		close(pipe1[0]);

		// Set standard output to pipe1
		close(1);
		dup(pipe1[1]);

		// Make call
		execlp("cat", "cat", "/etc/passwd", NULL);
	}
	else
	{
		if (!fork())
		{	// Process 1

			// Set standard input from pipe1
			close(0);
			dup(pipe1[0]);

			// set stdout to pipe2
			close(1);
			close(pipe1[1]);
			dup(pipe2[1]);

			// Make call
			execlp("cut", "cut", "-f1", "-d:", NULL);
		}
		else
		{	// Process 2

			// Set standard input from pipe2
			close(0);
			close(pipe1[0]);
			dup(pipe2[0]);

			// There is no output to pipe1 or pipe2
			close(pipe1[1]);
			close(pipe2[1]);

			// Make call
			execlp("sort", "sort", NULL);
		}
	}

	return 0;
}
