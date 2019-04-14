#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void wait_for_ready_children() {
	pid_t p;
	int status;
	p = waitpid(-1, &status, WUNTRACED);
	if (!WIFSTOPPED(status)) {
		fprintf(stderr, "Parent: Child with PID %ld has died unexpectedly!\n", (long)p);
		exit(1);
	}
}

void fork_procs()
{
	printf("2\n");
	raise(SIGSTOP);
	printf("4\n");
	exit(0);	
}


/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
 *
 * How to wait for the process tree to be ready?
 * In ask2-{fork, tree}:
 *      wait for a few seconds, hope for the best.
 * In ask2-signals:
 *      use wait_for_ready_children() to wait until
 *      the first process raises SIGSTOP.
 */

int main(int argc, char *argv[])
{
	printf("1\n");
	pid_t pid;
	int status;
	pid = fork();
	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}
	if (pid == 0) {
		fork_procs();
		exit(1);
	}

	wait_for_ready_children();
	printf("3\n");
	kill(pid, SIGCONT);
	printf("5\n");
	wait(&status);
	printf("6\n");
	return 0;
}
