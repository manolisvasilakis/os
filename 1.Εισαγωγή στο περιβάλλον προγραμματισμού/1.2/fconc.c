#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void helper(int, int, char *);

int main (int argc, char *argv[]){
	if (argc < 3 || argc > 4){
		printf("Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n");
		exit(1);
	}
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		printf("%s: No such file or directory\n", argv[1]);
		exit(1);
	}
	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1){
		close(fd1);
		printf("%s: No such file or directory\n", argv[2]);
		exit(1);
	}
	int fw;
	mode_t mode = S_IRUSR | S_IWUSR;
	if (argc == 4)	fw = creat(argv[3], mode);
	else if (argv[2] == argv[3]) {
			printf("Not acceptable input - output\n");
			exit(1);
	}
	else fw = creat("fconc.out", mode);
	if (fw == -1){
		close(fd1);
		close(fd2);
		printf("problem with output\n");
		exit(1);
	}
	char buff[1024];	//endiameso vhma
	helper(fd1, fw, buff);
	close(fd1);
	helper(fd2, fw, buff);
	close(fd2);
	return 0;
}

void helper(int in, int out, char *buff) {
	ssize_t counter;	//posa diavazo
	while(1){
		counter = read(in, buff, sizeof(buff) - 1);
		if (counter == 0) break;
		counter = write(out, buff, counter);
		if (counter == 0){
			printf("problem with write");
			exit(1);
		}
	}
}
