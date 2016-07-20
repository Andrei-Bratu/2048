build: 2048
2048: 2048.o
	gcc -Wall 2048.o -o 2048 -lncurses
2048.o: 2048.c
	gcc -Wall -c 2048.c -o 2048.o -lncurses
run: 2048
	./2048
clean: 2048
	rm 2048 2048.o
