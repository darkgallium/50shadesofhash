CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -O3 -g -fopenmp
LIBS=-lm -lssl -lcrypto

rainbow: table.o rainbow.o crypto.o io.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)

table.o: rainbow.h table.c table.h
	$(CC) $(CFLAGS) -o table.o -c table.c

crypto.o: crypto.c crypto.h
	$(CC) $(CFLAGS) -o crypto.o -c crypto.c

rainbow.o: rainbow.c rainbow.h
	$(CC) $(CFLAGS) -o rainbow.o -c rainbow.c

io.o: io.c io.h rainbow.h
	$(CC) $(CFLAGS) -o io.o -c io.c
clean:
	rm -rf rainbow *.o
