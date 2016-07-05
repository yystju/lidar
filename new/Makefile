CXX       = g++-4.8
CC        = gcc-4.8

CFLAGS    = -I. -std=c99 -Wno-implicit-function-declaration
CXXFLAGS  = -I. -I../libs/xsens/include -std=c++11
LIBS      = -L../libs/xsens/lib -llibxcommunication -llibxstypes -lm -lpthread

DEPS_H    = $(wildcard *.h)
OBJ       = main.o lidar.o xsens.o raspi.o helper.o xsens_api_wrapper.o

all: main

main: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

%.o: %.c $(DEPS_H)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp $(DEPS_H)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f *.o main *.log
