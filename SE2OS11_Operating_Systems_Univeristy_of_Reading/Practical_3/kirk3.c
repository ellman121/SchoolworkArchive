/*
** kirk3.c -- teleportation?
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

// Message Types
#define URGENT (1)
#define NORMAL (2)

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

volatile sig_atomic_t got_usr1;

void sigusr1_handler(int sig)
{
	got_usr1 = 1;
}

int main(void)
{
	struct sigaction sa;
	got_usr1 = 0;

	sa.sa_handler = sigusr1_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	struct my_msgbuf buf;
	int msqid;
	key_t key;

	if ((key = ftok("kirk.c", 'B')) == -1) {
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644 | IPC_CREAT)) == -1) {
		perror("msgget");
		exit(1);
	}
	
	printf("Enter lines of text, ^D to quit:\n");

	while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL && !got_usr1) {
		int len = strlen(buf.mtext);

		/* ditch newline at end, if it exists */
		if (buf.mtext[len-1] == '\n')
			buf.mtext[len-1] = '\0';

		/* Check and set message type */
		if (len > 6 && (strncmp("URGENT", buf.mtext, 6) == 0) )
			buf.mtype = URGENT;
		else
			buf.mtype = NORMAL;

		if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
			perror("msgsnd");
	}

	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}

	return 0;
}

