#!/bin/bash

CMAKE=/usr/bin/cmake
if [ "$TRAVIS_OS_NAME" == "osx" ]; then CMAKE=/usr/local/bin/cmake; fi
if [ ! -e build ]; then mkdir build; fi
cd build
$CMAKE --version
$CMAKE -G "Ninja" ..
ninja $@
