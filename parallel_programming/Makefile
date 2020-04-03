CC=gcc
SRC = mpi_lab_3.c
OBJ = mpi_lab_3.o
CFLAGS=-lmpi
TARGET=parallel_matrix_multiplication

all: main

main: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS)

$(OBJ): $(SRC)
	gcc -c $(SRC)

debug:
	$(CC) $(OBJ) -o $(TARGET) $(CFLAGS) -g

clean:
	rm -rf $(TARGET) *[0-9] *~*
