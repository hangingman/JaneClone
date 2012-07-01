################################################################################
#
# JaneCloneビルド用Makefile 
# Copyright (c) 2012 HIROYUKI Nagata <jagdironscrap@gmail.com>
#
################################################################################

TARGET  = JaneClone
SOURCES = $(notdir $(shell find . -name '*.cpp'))
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS +=$(RCS:.rc=.o)

# 基本コマンド
CXX		:= g++
RM 		:= rm

# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS	 =	-gstabs -O0
CXX_RELEASE_FLAGS	 =	-s -O0

# オプション
CXXFLAGS = -Wall -I/usr/local/include -I include `wx-config --cxxflags` `xml2-config --cflags`
LDFLAGS  = -L/usr/local/lib -lwx_gtk2u_aui-2.8 `wx-config --libs` `xml2-config --libs` -lmk4 ./libnkf.a
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

# clean
.PHONY: clean
clean:
	$(RM) -f *.o $(TARGET)