#!/bin/bash

rm -rf ./build
CMAKE=/usr/bin/cmake
if [ ! $(type -P ${CMAKE} 2>/dev/null) ]; then CMAKE=/usr/local/bin/cmake; fi
mkdir build
cd build
$CMAKE --version
$CMAKE -G "Ninja" ..
ninja $@
