out4: ex4.o proc-common.o tree.o
	gcc -o out4 ex4.o proc-common.o tree.o

ex4.o : ex4.c
	gcc -Wall ex4.c -c

proc-common.o: proc-common.c
	gcc -Wall proc-common.c -c

tree.o: tree.c
	gcc -Wall tree.c -c

clean: 
	rm ex4.o tree.o proc-common.o
