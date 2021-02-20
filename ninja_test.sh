#!/bin/bash
#
# cd ~/git/JaneClone && ./ninja_test.sh ..
#
if [ ! -e build ]; then mkdir build; fi
cd build
/usr/bin/cmake --version
/usr/bin/cmake -G "Ninja" -Dtest=on ..
/usr/bin/cmake --build .
ctest -V
