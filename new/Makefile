CXX       = g++-4.8
CC        = gcc-4.8

CFLAGS    = -I. -std=c99 -Wno-implicit-function-declaration
CXXFLAGS  = -I. -I../src_cpp/include -std=c++11
LIBS      = -L../src_cpp/xcommunication -L../src_cpp/xstypes -lxcommunication -lxstypes -lm -lpthread -luuid -lwiringPi

DEPS_H    = $(wildcard *.h)
OBJ       = helper.o repository.o raspi.o deviceclass.o xsens_api_wrapper.o xsens.o lidar.o main.o 

all: main

main: $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

%.o: %.c $(DEPS_H)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp $(DEPS_H)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f *.o main *.log
