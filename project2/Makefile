CC = gcc
CXX = g++
CFLAGS = -g -Wall
SHFLAGS = -shared -fPIC
LDL = -ldl
LARGE = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

PROGS = logger.cpp share.cpp test.cpp

all: $(PROGS)
	$(CXX) -o logger $(CFLAGS) logger.cpp	$(LDL)
	$(CXX) -o logger.so $(SHFLAGS) share.cpp $(LDL)
	$(CXX) $(LARGE) -o test $(CFLAGS) test.cpp
clean:
	rm -f -r logger.so logger *.dSYM *.txt core temp temp1 test
run:
	./logger ./test
	