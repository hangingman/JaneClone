#!/bin/bash

rm -rf ./build
mkdir build
cd build
/usr/bin/cmake --version
/usr/bin/cmake -G "Ninja" ..
ninja $@
