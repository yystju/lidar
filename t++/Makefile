CXX    = g++
CC     = gcc

CFLAGS = -I.-std=c99
CXXFLAGS = -I. -std=c++11
LIBS   = -lm

DEPS_HPP = $(wildcard *.hpp)
DEPS_H = $(wildcard *.h)
OBJ = main.o receive.o helper.o serial.o

all: main

main: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

%.o: %.cpp $(DEPS_HPP)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

%.o: %.c $(DEPS_H)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o main
