#!/bin/bash
#
# script for cross compile i686-w64-mingw32
#
./bootstrap

XML2CONFIG='/usr/i686-w64-mingw32/bin/xml2-config'   \
WX_CONFIG_PATH='/usr/i686-w64-mingw32/bin/wx-config' \
CURLCONFIG='/usr/i686-w64-mingw32/bin/curl-config'   \
./configure --prefix=/usr/i686-w64-mingw32           \
--build=x86_64-unknown-linux-gnu                     \
--host=i686-w64-mingw32                              \
--target=i686-w64-mingw32

make