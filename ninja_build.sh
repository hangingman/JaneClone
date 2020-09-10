#!/bin/bash

cd build
cmake -G "Ninja" ..
ninja $@
