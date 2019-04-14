#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void doWrite(int fd, char *buff, int len) {
		write(fd,buff,len);		//eggrafh toy buff ston fd
}		//na ftiaxtei to write edo me ta ypoloipa

void write_file(int output, int input, char *buff) {
	ssize_t rec;				//rec = plithos bytes poy diabasa
	rec = read(input, buff, 30);
	while (rec > 0) {
		doWrite(output, buff, rec);
		rec = read(input, buff, 30);
	}
	if (rec < 0) {
		perror("read");
		exit(1);
	}
}


int main(int argc, char *argv[]) {
	if(argc < 3 || argc > 4) {
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
		if (close(fd1) == -1) printf("close error\n");
		printf("%s: No such file or directory\n", argv[2]);
		exit(1);
	}
	int fw;
	mode_t mode = S_IRUSR | S_IWUSR;
	if (argc == 3)	fw = creat("fconc.out", mode);
	else if(strcmp(argv[1], argv[3]) == 0 || strcmp(argv[2], argv[3]) == 0){
		if (close(fd1) == -1) printf("close error\n");
		if (close(fd2) == -1) printf("close error\n");
		printf("ERROR: input file and output file cannot be the same\n");
		exit(1);
	}
	else fw = creat(argv[3], mode);
	if (fw == -1){
		if (close(fd1) == -1) printf("close error\n");
		if (close(fd2) == -1) printf("close error\n");
		printf("problem with output file\n");
		exit(1);
	}
	char *buff = (char *) malloc(30 * sizeof(char));
	write_file(fw, fd1, buff);
	if(close(fd1) == -1) {
		perror("close error");
		exit(1);
	}
	write_file(fw, fd2, buff);
	free(buff);
	if(close(fd2) == -1) {
		perror("close error");
		exit(1);
	}
	if(close(fw) == -1) {
		perror("close error");
		exit(1);
	}
	return 0;
}
