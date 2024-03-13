CC=gcc
CFLAGS=-Wall -Wextra -g

all: folders ficha1 ficha2 ficha3

ficha1 : bin/ficha_1

ficha2 : bin/ficha_2

ficha3 : bin/ficha_3

folders:
	@mkdir -p obj bin tmp

bin/ficha_1: obj/ficha1.o
	$(CC) $(LDFLAGS) $^ -o $@

bin/ficha_2: obj/ficha2.o
	$(CC) $(LDFLAGS) $^ -o $@

bin/ficha_3: obj/ficha3.o
	$(CC) $(LDFLAGS) $^ -o $@

ficha_1: ficha1/ficha1.c
	$(CC) $(CFLAGS) -o ficha1 ficha1/ficha1.c

ficha_2: ficha2/ficha2.c
	$(CC) $(CFLAGS) -o ficha2 ficha2/ficha2.c

ficha_3: ficha3/ficha3.c
	$(CC) $(CFLAGS) -o ficha3 ficha3/ficha3.c

obj/%.o: ficha1/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: ficha2/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: ficha3/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/* tmp/* bin/*