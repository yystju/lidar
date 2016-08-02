[steps to get started]
 $ cd xsens
 $ make
 $ cd ../node
 $ npm install
 $ cd ..
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make

[libuuid]
sudo apt-get install uuid-dev

[MTi sdk prerequisites]
sudo apt-get install realpath -y
sudo apt-get install sharutils -y
sudo apt-get install liblapack3 -y


[gcc crosscompiler for x86|x64]
sudo apt-get install gcc-arm-linux-gnueabihf -y
sudo apt-get install g++-arm-linux-gnueabihf -y

#sudo apt-get install gcc-aarch64-linux-gnu -y
#sudo apt-get install g++-aarch64-linux-gnu -y

sudo make ARCH=arm CC=arm-linux-gnueabihf-gcc CXX=arm-linux-gnueabihf-g++

#sudo make ARCH=aarch64 CC=aarch64-linux-gnu-gcc CXX=aarch64-linux-gnu-g++

[gcc-4.8 on Raspberry PI]
sudo apt-get install gcc-4.8 g++-4.8

[CMake with WiringPI]
/usr/share/cmake-3.x/Modules/FindWiringPi.cmake

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)


[CMake with libuuid]
/usr/share/cmake-3.x/Modules/FindUUID.cmake

find_library(UUID_LIBRARIES NAMES uuid)
find_path(UUID_INCLUDE_DIRS NAMES uuid.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(uuid DEFAULT_MSG UUID_LIBRARIES UUID_INCLUDE_DIRS)


[tcpdump]
tcpdump -i eth0 -n -x udp port 10110 -s 0 -c 1 -w dump.pcap
