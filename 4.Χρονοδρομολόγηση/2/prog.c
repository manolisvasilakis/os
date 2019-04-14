#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "proc-common.h"

#define NMSG 200
#define DELAY 130

int main(int argc, char *argv[])
{
	int i, delay, pid;

	/*
	 * Print a number of messages,
	 * use a random delay, so that processes terminate
	 * in random order.
	 */

	pid = getpid();
	srand(pid);
	delay = 30 + ((double)rand() / RAND_MAX) * DELAY;
	printf("%s: Starting, NMSG = %d, delay = %d\n",
		argv[0], NMSG, delay);

	for (i = 0; i < NMSG; i++) {
		printf("%s[%d]: This is message %d\n", argv[0], pid, i);
		compute(delay);
	}

	return 0;
}

/*

int main (int argc, char *argv[])
{
	printf("HELLO\n");
	
	char *hey[] = {"one", "two"};
	//char *newargv[] = { hey[0], NULL, NULL, NULL };
	printf("%s\n%s\n", hey[0], hey[1]);
	return 0;
}
*/
