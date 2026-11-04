CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2

all: basm debasm bme

# String view object
sv.o: sv.c sv.h
	$(CC) $(CFLAGS) -c sv.c

# Core VM object
bm.o: bm.c bm.h
	$(CC) $(CFLAGS) -c bm.c

# Assembler executable
basm: basm.c sv.o bm.o
	$(CC) $(CFLAGS) -o basm basm.c sv.o bm.o

debasm: debasm.c bm.o
	$(CC) $(CFLAGS) -o debasm debasm.c bm.o
# Interpreter executable
bme: bme.c bm.o
	$(CC) $(CFLAGS) -o bme bme.c bm.o

clean:
	rm -f *.o basm bme