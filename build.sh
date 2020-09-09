#!/bin/bash

cd build
cmake -G "Ninja" ..
ninja -k 0
