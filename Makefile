CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -O2

all: ebasm bmi

# String view object
sv.o: sv.c sv.h
	$(CC) $(CFLAGS) -c sv.c

# Core VM object
bm.o: bm.c bm.h
	$(CC) $(CFLAGS) -c bm.c

# Assembler executable
ebasm: ebasm.c sv.o bm.o
	$(CC) $(CFLAGS) -o ebasm ebasm.c sv.o bm.o

# Interpreter executable
bmi: bmi.c bm.o
	$(CC) $(CFLAGS) -o bmi bmi.c bm.o

clean:
	rm -f *.o ebasm bmi