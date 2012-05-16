################################################################################
#
# JaneCloneビルド用Makefile 
# Copyright (c) 2012 HIROYUKI Nagata <jagdironscrap@gmail.com>
#
################################################################################

TARGET 	= JaneClone
OBJECTS = Main.o JaneClone.o ExtractBoardList.o SocketCommunication.o         \
		  MetakitAccessor.o icon_rc.o JaneCloneUtil.o

# 基本コマンド
CXX		:= g++
RM 		:= rm
WINDRES := windres

# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-gstabs -O0
CXX_RELEASE_FLAGS	=	-s -O0

# オプション
CPPFLAGS = -Wall -I/c/MinGW/include -I include `wx-config --cxxflags` `xml2-config --cflags`
LDFLAGS  = -static -L/c/MinGW/lib -lwx_mswu_aui-2.9 `wx-config --libs` `xml2-config --libs` -lws2_32 -lz -lmk4
VPATH    = include src gimite rc

# デバッグ
.PHONY	: Debug
Debug 	: CXX+=$(CXX_DEBUG_FLAGS)
Debug 	: all
# リリース
.PHONY	: Release
Release	: CXX+=$(CXX_RELEASE_FLAGS)
Release	: all

all : $(TARGET)
$(TARGET) : $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)
icon_rc.o : icon.rc
		$(WINDRES) -i rc/"icon.rc" -O coff -o "icon_rc.o" -I/mingw/include/wx-2.9
MetakitAccessor.o : MetakitAccessor.cpp MetakitAccessor.h
		$(CXX) -c $< $(CPPFLAGS)
JaneCloneUtil.o : JaneCloneUtil.cpp JaneCloneUtil.h
		$(CXX) -c $< $(CPPFLAGS)
SocketCommunication.o : SocketCommunication.cpp SocketCommunication.h JaneCloneUtil.h
		$(CXX) -c $< $(CPPFLAGS)
ExtractBoardList.o : ExtractBoardList.cpp ExtractBoardList.h MetakitAccessor.h
		$(CXX) -c $< $(CPPFLAGS)
JaneCloneFrame.o : JaneClone.cpp JaneClone.h　ExtractBoardList.h SocketCommunication.h DataType.h MetakitAccessor.h
		$(CXX) -c $< $(CPPFLAGS)
Main.o : Main.cpp JaneClone.h
		$(CXX) -c $< $(CPPFLAGS)
.PHONY: clean
clean:
	$(RM) -f *.o *.exe