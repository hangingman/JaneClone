#!/bin/bash
#
# script for cross compile x86_64-w64-mingw32
#
./bootstrap

XML2CONFIG='/usr/x86_64-w64-mingw32/bin/xml2-config'   \
WX_CONFIG_PATH='/usr/x86_64-w64-mingw32/bin/wx-config' \
CURLCONFIG='/usr/x86_64-w64-mingw32/bin/curl-config'   \
./configure --prefix=/usr/x86_64-w64-mingw32           \
--build=x86_64-unknown-linux-gnu                       \
--host=x86_64-w64-mingw32                              \
--target=x86_64-w64-mingw32

make