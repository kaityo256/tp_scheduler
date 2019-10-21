SRC=$(shell ls *.cpp)
OBJ=$(SRC:.cpp=.o)
CC=g++
CPPFLAGS=-O3 -Wall -Wextra -std=c++11 -c

all: serial.out mpi.out


serial.out: main_serial.o ising_mc.o
	$(CC) $(LDFLAGS) $^ -o $@

mpi.out: main_mpi.o ising_mc.o
	$(CC) $(LDFLAGS) $^ -o $@ -lmpi

%.o: %.cpp
	g++ $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) serial.out mpi.out
