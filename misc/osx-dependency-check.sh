#!/bin/bash

#
# MacOSX上でJaneCloneをビルドした際のJaneCloneとライブラリの依存関係を出力します
#

# 場所が違うなら書き換えること
BUNDLE_PATH=../JaneClone.app/Contents/MacOS
OSX_JANECLONE_EXE=${BUNDLE_PATH}/JaneClone

echo "==============================================================="
echo "=              JaneCloneの依存関係を出力します　　　　　　　　　　　　 ="
echo "==============================================================="

otool -L ${OSX_JANECLONE_EXE}

echo "==============================================================="
echo "=              ライブラリの依存関係を出力します 　　　　　　　　　　　　 ="
echo "==============================================================="

for f in $(ls ${BUNDLE_PATH}/lib*.dylib); do otool -L ${f}; done
