CC = gcc
CFLAGS = -g -lncurses

build:battleship

battleship: battleship.c functions.c
	$(CC) -o battleship battleship.c functions.c $(CFLAGS)

run: battleship Board.txt
	./battleship Board.txt

Board.txt:
	gcc -o random random_gen.c
	./random > Board.txt
	rm -f random

clean:
	rm -f battleship
	rm -f *.out

