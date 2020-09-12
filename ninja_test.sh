#!/bin/bash
#
# cd ~/git/JaneClone && ./ninja_test.sh ..
#
if [ ! -e build ]; then mkdir build; fi
cd build
cmake -G "Ninja" -Dtest=on ..
cmake --build .
ctest -V
