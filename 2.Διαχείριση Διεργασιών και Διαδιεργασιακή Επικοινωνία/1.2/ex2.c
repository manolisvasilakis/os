#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tree.h"

#define SLEEP_PROC_SEC  2

void leaf_process(char *name, int level){
	int i;
	for (i=0; i<level; i++)	printf("\t");
	printf("%s : Sleeping...\n", name);
	sleep(SLEEP_PROC_SEC);
	for (i=0; i<level; i++)	printf("\t");
	printf("%s : Exiting...\n", name);
	exit(10);
}

void generate_process(struct tree_node *root, int level){
	if (root->nr_children == 0) leaf_process(root->name, level);

	pid_t pid;
	int i, status;

	for (i=0; i<level; i++)	printf("\t");
	printf("%s : Making children...\n", root->name);

	for (i=0; i < root->nr_children; i++){
		pid = fork();
		if (pid == -1) {
			printf("fork unsuccessful\n");
			exit(1);
		}
		else if (pid == 0) generate_process(root->children + i, level + 1);
		wait(&status);	// depth first dhmiourgia paidion
	}
	//for (i=0; i < root->nr_children; i++) wait(&status);	// enallaktika ftiaxno ola ta paidia sthn arxh kai perimeno na gyrisoun
	for (i=0; i<level; i++) printf("\t");
	printf("%s : Exiting...\n", root->name);
	exit(11);
}

int main(int argc, char *argv[]){
	struct tree_node *root;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
		exit(1);
	}

	root = get_tree_from_file(argv[1]);
	int status;
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		printf("fork unsuccessful\n");
		exit(1);
	}
	else if (pid == 0) generate_process(root, 0);
	wait(&status);
	return 0;
}
