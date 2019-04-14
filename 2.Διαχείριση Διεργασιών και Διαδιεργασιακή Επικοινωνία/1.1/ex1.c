#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "proc-common.h"

#define SLEEP_PROC_SEC  10
#define SLEEP_TREE_SEC  3

/*
 * Create this process tree:
 * A-+-B---D
 *   `-C
 */

 void Dfork_procs(){
 	change_pname("D");
 	printf("D : Sleeping...\n");
 	sleep(SLEEP_PROC_SEC);
 	printf("D : Exiting...\n");
 	exit(13);
 }

void Cfork_procs() {
	change_pname("C");
	printf("C : Sleeping...\n");
	sleep(SLEEP_PROC_SEC);
	printf("C : Exiting...\n");
	exit(17);
}

 void Bfork_procs(){
 	change_pname("B");
 	printf("B : Starting...\n");

 	pid_t pid;
	int status;

	/* Fork D */
	pid = fork();
	if (pid < 0) {
		perror("B: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child */
		Dfork_procs();
		assert(0);
	}
	printf("B: Waiting for children...\n");
	pid = wait(&status);
	explain_wait_status(pid, status);

	printf("B : Exiting...\n");
	exit(19);

 }

void fork_procs() {
	/*
	 * initial process is A.
	 */
	change_pname("A");
	printf("A: Starting..\n");

	pid_t pid;
	int status;

	/* Fork B */
	pid = fork();
	if (pid < 0) {
		perror("A: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child */
		Bfork_procs();
		assert(0);
	}

	/* Fork C */
	pid = fork();
	if (pid < 0) {
		perror("A: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child */
		Cfork_procs();
		assert(0);
	}

	printf("A: Waiting for children...\n");
	pid = wait(&status);
	explain_wait_status(pid, status);
	pid = wait(&status);
	explain_wait_status(pid, status);

	printf("A: Exiting...\n");
	exit(16);
}

int main() {
	pid_t pid;
	int status;

	/* Fork root of process tree */
	pid = fork();
	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child */
		fork_procs();
		assert(0);
	}

	/*
	 * Father
	 */
	sleep(SLEEP_TREE_SEC);

	/* Print the process tree root at pid */
	show_pstree(pid);
	//show_pstree(getpid());
	/* Wait for the root of the process tree to terminate */
	pid = wait(&status);
	explain_wait_status(pid, status);
	return 0;
}
