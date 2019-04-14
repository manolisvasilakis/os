#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "tree.h"
#include "proc-common.h"

void leaf_procs(struct tree_node *root){
	printf("PID = %ld, name %s, is starting...\n", (long)getpid(), root->name);
	change_pname(root->name);
	raise(SIGSTOP);
	printf("PID = %ld, name = %s is awake and exiting\n", (long)getpid(), root->name);
	exit(0);
}

void fork_procs(struct tree_node *root) {
	if (root->nr_children == 0) leaf_procs(root);

	printf("PID = %ld, name %s, is starting...\n", (long)getpid(), root->name);
	change_pname(root->name);

	pid_t pid;
	int status, i;
	pid_t children[root->nr_children];		//an eimai leaf orizo pinaka 0 theseon
	for (i=0; i < root->nr_children; i++){
		pid = fork();
		if (pid == -1) {
			printf("fork unsuccessful\n");
			exit(1);
		}
		else if (pid == 0) fork_procs(root->children + i);
		children[i] = pid;
		wait_for_ready_children(1);			//depth first dhmiourgia
	}

	//wait_for_ready_children(root->nr_children);	//an den me noiazei h seira dhmiourgias

	/* Suspend Self */
	raise(SIGSTOP);
	printf("PID = %ld, name = %s is awake\n", (long)getpid(), root->name);

	for (i=0; i < root->nr_children; i++) {
		kill(children[i], SIGCONT);
	//}
	//for (i=0; i < root->nr_children; i++) {
		pid = wait(&status);
		explain_wait_status(pid, status);			//depth first awakening
	}

	printf("PID = %ld, name = %s is exiting\n", (long)getpid(), root->name);
	/* Exit */
	exit(0);
}

int main(int argc, char *argv[]) {
	pid_t pid;
	int status;
	struct tree_node *root;

	if (argc != 2){
		fprintf(stderr, "Usage: %s <tree_file>\n", argv[0]);
		exit(1);
	}

	/* Read tree into memory */
	root = get_tree_from_file(argv[1]);

	/* Fork root of process tree */
	pid = fork();
	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}
	else if (pid == 0) {
		/* Child */
		fork_procs(root);
		assert(0);
	}

	/* Father */
	wait_for_ready_children(1);

	/* Print the process tree root at pid */
	show_pstree(pid);

	/* for ask2-signals */
	kill(pid, SIGCONT);

	/* Wait for the root of the process tree to terminate */
	pid = wait(&status);
	explain_wait_status(pid, status);

	return 0;
}
