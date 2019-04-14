out1 : ex1.o proc-common.o
	gcc -o out1 ex1.o proc-common.o

ex1.o : ex1.c
	gcc -Wall -c ex1.c

proc-common.o : proc-common.c
	gcc -Wall -c proc-common.c

clean :
	rm ex1.o proc-common.o
