CC=gcc
SRC = main.c
CFLAGS=-lm
EXEC=multiprocess

all:
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS)

debug:
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) -g

clean:
	rm -rf $(EXEC) *[0-9] *~*
