################################################################################
#
# JaneCloneビルド用Makefile 
# Copyright (c) 2012 HIROYUKI Nagata <jagdironscrap@gmail.com>
#
################################################################################

TARGET  = JaneClone
SOURCES = $(notdir $(shell find . -name '*.cpp'))
RCS		= $(notdir $(shell find . -name '*.rc'))
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS +=$(RCS:.rc=.o)

# 基本コマンド
CXX		:= g++
RM 		:= rm
WINDRES := windres

# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-gstabs -O0
CXX_RELEASE_FLAGS	=	-s -O0

# オプション
CXXFLAGS = -Wall -I/c/MinGW/include -I include `wx-config --cxxflags` `xml2-config --cflags`
RCFLAGS  = $(shell wx-config --cxxflags | awk '{ print $$2 }')  # wx-configからwxのヘッダファイルのルートディレクトリを取り出している
LDFLAGS  = -static -L/c/MinGW/lib -lwx_mswu_aui-2.9 `wx-config --libs` `xml2-config --libs` -liconv -lmk4
VPATH    = include src rc

# make all
all : $(TARGET)
$(TARGET): $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)

# suffix rule
.SUFFIXES: .cpp .o
.cpp.o:
		$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

# debug
.PHONY	: Debug
Debug 	: CXX+=$(CXX_DEBUG_FLAGS)
Debug 	: all
# release
.PHONY	: Release
Release	: CXX+=$(CXX_RELEASE_FLAGS)
Release	: all

# icon build
icon.o : icon.rc
		$(WINDRES) -i $^ -O coff -o $@  $(RCFLAGS)

# clean
.PHONY: clean
clean:
	$(RM) -f *.o $(TARGET).exe