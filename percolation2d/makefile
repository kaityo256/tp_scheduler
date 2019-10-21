SRC=$(shell ls *.cpp)
OBJ=$(SRC:.cpp=.o)
CC=g++
CPPFLAGS=-O3 -Wall -Wextra -std=c++11

-include makefile.opt

all: a.out

a.out: percolation2d.cpp
	$(CC) $(CPPFLAGS) $< -o $@


clean:
	rm -f a.out
