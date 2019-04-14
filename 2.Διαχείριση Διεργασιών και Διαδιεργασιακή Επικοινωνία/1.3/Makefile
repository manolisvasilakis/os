out3: ex3.o proc-common.o tree.o
	gcc -o out3 ex3.o proc-common.o tree.o

ex3.o : ex3.c
	gcc -Wall ex3.c -c

proc-common.o: proc-common.c
	gcc -Wall proc-common.c -c

tree.o: tree.c
	gcc -Wall tree.c -c

clean: 
	rm ex3.o tree.o proc-common.o
