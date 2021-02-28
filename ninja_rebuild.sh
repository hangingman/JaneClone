#!/bin/bash

rm -rf ./build
CMAKE=/usr/bin/cmake
if [ "$TRAVIS_OS_NAME" == "osx" ]; then CMAKE=/usr/local/bin/cmake; fi
mkdir build
cd build
$CMAKE --version
$CMAKE -G "Ninja" ..
ninja $@
