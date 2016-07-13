CXX       = g++
CC        = gcc

CFLAGS    = -I. -std=c99 -Wno-pointer-sign -Wno-deprecated-declarations
CXXFLAGS  = -I. -std=c++11
LIBS      = 

DEPS_H    = $(wildcard *.h)
OBJ       = aes.o test.o 

all: test

test: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

%.o: %.c $(DEPS_H)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp $(DEPS_H)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f *.o test
