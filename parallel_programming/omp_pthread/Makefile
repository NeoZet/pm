CC=gcc
SRC = omp_pthread.c
OBJ = omp_pthread.o
CFLAGS=-lpthread -fopenmp -O0
TARGET=omp_pthread

all: main

main: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS)

$(OBJ): $(SRC)
	$(CC) -c $(SRC)

debug:
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS) -g

clean:
	rm -rf $(TARGET) *[0-9] *~* *.o
