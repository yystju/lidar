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
