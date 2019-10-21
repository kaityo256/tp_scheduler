SRC=$(shell ls *.cpp)
OBJ=$(SRC:.cpp=.o)
CC=g++
CPPFLAGS=-O3 -Wall -Wextra -std=c++11

-include makefile.opt

all: serial.out mpi.out

serial.out: main_serial.o percolation2d.o
	$(CC) $^ -o $@

mpi.out: main_mpi.o percolation2d.o
	$(CC) $^ -o $@ -lmpi


%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

test:
	time ./serial.out | sort -nk1 > serial.dat
	time mpirun -np 40 ./mpi.out | sort -nk1 > mpi.dat
	diff serial.dat mpi.dat

clean:
	rm -f serial.out
