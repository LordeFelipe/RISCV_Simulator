main: main.o memory.o
	gcc main.o memory.o -g -o main

main.o: main.c 
	gcc -g -c main.c

memory.o: memory.c
	gcc -g -c memory.c 