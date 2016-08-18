CXX       = g++
CC        = gcc

CFLAGS    = -I. -std=c99 -Wno-implicit-function-declaration
CXXFLAGS  = -I. -I../xsens/include -std=c++11

DEPS_H    = $(wildcard *.h)

LIBS      = 

all: test test1 test2

test: test.c
#	$(CC) -o test test.c $(CFLAGS) $(LIBS)
	
test1: test1.c
	$(CC) -o test1 test1.c $(CFLAGS) $(LIBS)
	
test2: test2.c
	$(CC) -o test2 test2.c $(CFLAGS) $(LIBS)

clean:
	rm -f *.o test test1 test2
