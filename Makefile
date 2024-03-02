CC=gcc
CFLAGS=-Wall -Wextra

all: ficha1 ficha2

ficha1: ficha1.c
	$(CC) $(CFLAGS) -o ficha1 ficha1.c

ficha2: ficha2.c
	$(CC) $(CFLAGS) -o ficha2 ficha2.c

clean: 
	rm -f ficha1 ficha2