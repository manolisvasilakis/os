#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include "proc-common.h"
#include "tree.h"

#define SLEEP_PROC_SEC  2

void leaf_process(char *name, int fd){
	printf("%s : Writing to pipe...\n", name);
	int num = atoi(name);
	if (write(fd, &num, sizeof(num)) != sizeof(num)) {
		perror("child: write to pipe");
		exit(1);
	}
	close(fd);
	exit(num);
}

void child_ps(struct tree_node *root, int level, int fd) {
	int i;
	for (i=0; i<level; i++)	printf("\t");

	if (root->nr_children == 0) leaf_process(root->name, fd);

	pid_t pid;
	int pfd[2];
	int status;

	printf("%s: Creating pipe...\n", root->name);
	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}
	for (i=0; i<level; i++)	printf("\t");
	printf("%s: Creating childen...\n", root->name);
	for (i=0; i<2; i++){
		pid = fork();
		if (pid < 0) {
			/* fork failed */
			perror("fork");
			exit(1);
		}
		if (pid == 0) {
			close(fd);
			close(pfd[0]);	//does not use those, uses only new writing end
			child_ps(root->children + i, level + 1, pfd[1]);
			assert(0);
		}
	}

	/* Father */
	close(pfd[1]);	//does not write here

	pid = wait(&status);
	explain_wait_status(pid, status);
	pid = wait(&status);
	explain_wait_status(pid, status);	//den xreiazontai

	int num1, num2;
	if (read(pfd[0], &num1, sizeof(num1)) != sizeof(num1)) {
		perror("Parent: read from pipe");
		exit(1);
	}

	if (read(pfd[0], &num2, sizeof(num2)) != sizeof(num2)) {
		perror("Parent: read from pipe");
		exit(1);
	}

	close(pfd[0]);	//read everything
	int result;
	if (strcmp(root->name, "+") == 0) result = num1 + num2;
	else result = num1 * num2;

	if (write(fd, &result, sizeof(result)) != sizeof(result)) {
		perror("child: write to pipe");
		exit(1);
	}
	close(fd);
	for (i=0; i<level; i++) printf("\t");
	printf("%s : Exiting...\n", root->name);
	exit(result);
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
	pid_t pid;

	printf("Main: Creating pipe...\n");
	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}

	printf("Main: Creating root...\n");
	pid = fork();
	if (pid < 0) {
		/* fork failed */
		perror("Main: fork");
		exit(1);
	}
	if (pid == 0) {
		/* In child process */
		close(pfd[0]);	//does not read
		child_ps(root, 0, pfd[1]);
		assert(0);
	}
	close(pfd[1]);	//does not write
	int result;
	printf("Main: Created root with PID = %ld, waiting for it to terminate...\n", (long)pid);
	pid = wait(&status);
	explain_wait_status(pid, status);
	if (read(pfd[0], &result, sizeof(result)) != sizeof(result)) {
		perror("Main: read from pipe");
		exit(1);
	}
	close(pfd[0]);
	printf("\n\nResult = %d\n", result);
	return 0;
}

