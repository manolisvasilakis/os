#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "proc-common.h"
#include "tree.h"

#define SLEEP_PROC_SEC  2


void leaf_process(char *name, int fd){
	printf("%s : Sleeping...\n", name);
	sleep(SLEEP_PROC_SEC);
	int num = atoi(name);
	if (write(fd, &num, sizeof(num)) != sizeof(num)) {
		perror("child: write to pipe");
		exit(1);
	}
	close(fd);
	exit(10);
}

static void child_ps(struct tree_node *root, int level, int read_from, int write_to) {
	int i;
	for (i=0; i<level; i++)	printf("\t");
	
	if (root->nr_children == 0) {
		close(read_from);
		leaf_process(root->name, write_to);
	}
	
	pid_t p;
	int status;
	
	printf("%s: Creating childen...\n", root->name);
	p = fork();
	if (p < 0) {
		/* fork failed */
		perror("fork");
		exit(1);
	}
	if (p == 0) {
		child_ps(root->children, level + 1, read_from, write_to);
		/*
		 * Should never reach this point,
		 * child() does not return
		 */
		assert(0);
	}
	
	int fd[2];	
	if (pipe(fd) < 0) {
		perror("pipe");
		exit(1);
	}
	for (i=0; i<level; i++)	printf("\t");
	p = fork();
	if (p < 0) {
		/* fork failed */
		perror("fork");
		exit(1);
	}
	if (p == 0) {
		close(read_from);
		close(write_to);	//does not use previous pipe
		child_ps(root->children + 1, level + 1, fd[0], fd[1]);
		/*
		 * Should never reach this point,
		 * child() does not return
		 */
		assert(0);
	}	
	close(fd[1]);			//parent does not write here
	
	p = wait(&status);
	explain_wait_status(p, status);
	p = wait(&status);
	explain_wait_status(p, status);
	int num1, num2;
	if (read(read_from, &num1, sizeof(num1)) != sizeof(num1)) {
		perror("Parent: read from pipe");
		exit(1);
	}
	close(read_from);
	
	if (read(fd[0], &num2, sizeof(num2)) != sizeof(num2)) {
		perror("Parent: read from pipe");
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);			//does not use further this pipe
	int result;
	if (strcmp(root->name, "+") == 0) result = num1 + num2;
	else result = num1 * num2;
	if (write(write_to, &result, sizeof(result)) != sizeof(result)) {
		perror("child: write to pipe");
		exit(1);
	}
	close(write_to);
	for (i=0; i<level; i++) printf("\t");
	printf("%s : Exiting...\n", root->name);
	exit(11);
}

int main(int argc, char *argv[]) {
	struct tree_node *root;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
		exit(1);
	}

	root = get_tree_from_file(argv[1]);
	
	int pfd[2];
	int status;
	pid_t p;
	
	printf("Main: Creating pipe...\n");
	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}
	
	printf("Main: Creating root...\n");
	p = fork();
	if (p < 0) {
		/* fork failed */
		perror("Main: fork");
		exit(1);
	}
	if (p == 0) {
		/* In child process */
		child_ps(root, 0, pfd[0], pfd[1]);
		assert(0);
	}
	close(pfd[1]);	//main does not write
	int result;
	printf("Main: Created root with PID = %ld, waiting for it to terminate...\n", (long)p);
	p = wait(&status);
	explain_wait_status(p, status);
	if (read(pfd[0], &result, sizeof(result)) != sizeof(result)) {
		perror("Main: read from pipe");
		exit(1);
	}	
	close(pfd[0]);
	printf("\n\nResult = %d\n", result);
	return 0;
}

