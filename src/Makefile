CXX       = g++-4.8
CC        = gcc-4.8

CFLAGS    = -I. -std=c99 -Wno-implicit-function-declaration
CXXFLAGS  = -I. -I../xsens/include -std=c++11

DEPS_H    = $(wildcard *.h)

LIBS      = -L../xsens/xcommunication -L../xsens/xstypes -lxcommunication -lxstypes -lm -lpthread -luuid -lwiringPi
OBJ       = helper.o aes.o repository.o raspi.o serial.o deviceclass.o xsens_api_wrapper.o xsens.o lidar.o main.o 

all: main

main: $(OBJ)
	$(MAKE) -C ../xsens $(MFLAGS)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

%.o: %.c $(DEPS_H)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp $(DEPS_H)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f *.o main *.log
	$(MAKE) -C ../xsens clean
