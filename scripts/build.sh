#!/bin/sh

cd xsens

make

cd ..

mkdir -p build

cd build

cmake ..

make

make package
