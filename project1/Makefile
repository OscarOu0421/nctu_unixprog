CC = gcc
CXX = g++
CFLAGS = -g -Wall

PROGS = $(patsubst %.cpp, %, $(wildcard *.cpp))

all: $(PROGS)

%: %.c
	$(CC) -o $@ $(CFLAGS) $<

%: %.cpp
	$(CXX) -o $@ $(CFLAGS) $<

clean:
	rm -f -r $(PROGS) *.dSYM