/*
** starfleet.c -- reads from a message queue
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Message Types
#define URGENT (1)
#define NORMAL (2)

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid;
	key_t key;

	if ((key = ftok("kirk.c", 'B')) == -1) {  /* same key as kirk.c */
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644)) == -1) { /* connect to the queue */
		perror("msgget");
		exit(1);
	}
	
	printf("starfleet: ready to receive messages, captain.\n");

	for(;;) { /* Spock never quits! */
		if (msgrcv(msqid, &buf, sizeof buf.mtext, URGENT, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		printf("starfleet: \"%s\"\n", buf.mtext);
	}

	return 0;
}

