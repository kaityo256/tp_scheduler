SRC=$(shell ls *.cpp)
OBJ=$(SRC:.cpp=.o)
CC=g++
CPPFLAGS=-O3 -Wall -Wextra -std=c++11 -c

-include makefile.opt

all: serial.out mpi.out


serial.out: main_serial.o ising_mc.o
	$(CC) $(LDFLAGS) $^ -o $@

mpi.out: main_mpi.o ising_mc.o
	$(CC) $(LDFLAGS) $^ -o $@ -lmpi

%.o: %.cpp
	g++ $(CPPFLAGS) $< -o $@

test: serial.out mpi.out
	time ./serial.out > serial.dat
	time mpirun -np 40 ./mpi.out | sort -nk 1 > mpi.dat
	diff serial.dat mpi.dat

.PHONY: clean

clean:
	rm -f $(OBJ) serial.out mpi.out
