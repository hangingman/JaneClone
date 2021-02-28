#!/bin/bash

CMAKE=/usr/bin/cmake
if [ ! $(type -P ${CMAKE} 2>/dev/null) ]; then CMAKE=/usr/local/bin/cmake; fi
if [ ! -e build ]; then mkdir build; fi
cd build
$CMAKE --version
$CMAKE -G "Ninja" -Dtest=on ..
$CMAKE --build .
ctest -V
