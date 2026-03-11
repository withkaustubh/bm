CFLAGS = -Wall -Wextra

all : bm
bm : bm.c
	$(CC) $(CFLAGS) -o bm bm.c

.PHONY: all clean

clean :
	rm -f bm