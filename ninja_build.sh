#!/bin/bash

if [ ! -e build ]; then mkdir build; fi
cd build
/usr/bin/cmake --version
/usr/bin/cmake -G "Ninja" ..
ninja $@
