CXX       = g++-4.8
CC        = gcc-4.8

CFLAGS    = -I. -std=c99 -Wno-implicit-function-declaration
CXXFLAGS  = -I. -I../src_cpp/include -std=c++11

DEPS_H    = $(wildcard *.h)

#-- For real build --
LIBS      = -L../src_cpp/xcommunication -L../src_cpp/xstypes -lxcommunication -lxstypes -lm -lpthread -luuid -lwiringPi
OBJ       = helper.o aes.o repository.o raspi.o deviceclass.o xsens_api_wrapper.o xsens.o lidar.o main.o 

#-- Only for development --
#LIBS      = -lm -lpthread -luuid
#OBJ       = helper.o repository.o aes.o main.o

all: main

main: $(OBJ)
	$(MAKE) -C ../src_cpp $(MFLAGS)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

%.o: %.c $(DEPS_H)
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp $(DEPS_H)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f *.o main *.log
	$(MAKE) -C ../src_cpp clean
