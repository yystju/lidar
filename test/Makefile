CC     = gcc
CXX    = g++

CFLAGS = -I. -std=c99
LIBS   = -lm

DEPS = send_gprmc.h
OBJ = main.o send_gprmc.o

all: main

main: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o test