TARGET 	= JaneClone
OBJECTS = Main.o JaneClone.o ExtractBoardList.o SocketCommunication.o SQLiteBundle.o icon_rc.o

# 基本コマンド
RM 	:= rm
CXX 	:= g++
CC 	:= g++
WINDRES := windres

# デバッグ時とリリース時の微調整
# 最適化は実行しないことにした。最適化を行うとiconvが実行されなくなる。
CXX_DEBUG_FLAGS		=	-g -O0
CXX_RELEASE_FLAGS	=	-s -O0

# オプション
CPPFLAGS = -Wall -D __WXMSW__ -I/c/MinGW/include -I/c/MinGW/include -I include `wx-config --cxxflags` `xml2-config --cflags`
LDFLAGS  = -static -L/c/MinGW/lib `wx-config --libs` `xml2-config --libs` -lws2_32 -lz -lwxcode_mswu_wxsqlite3-2.9 -lsqlite3
VPATH    = include src gimite rc

# デバッグ
.PHONY	: Debug
Debug 	: CXXFLAGS+=$(CXX_DEBUG_FLAGS)
Debug 	: all
# リリース
.PHONY	: Release
Release	: CXXFLAGS+=$(CXX_RELEASE_FLAGS)
Release	: all

all : $(TARGET)
$(TARGET) : $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)
icon_rc.o : icon.rc
		$(WINDRES) -i rc/"icon.rc" -O coff -o "icon_rc.o" -I/mingw/include/wx-2.9
SQLiteBundle.o : SQLiteBundle.cpp SQLiteBundle.h
		$(CXX) -c $< $(CPPFLAGS)
SocketCommunication.o : SocketCommunication.cpp SocketCommunication.h
		$(CXX) -c $< $(CPPFLAGS)
ExtractBoardList.o : ExtractBoardList.cpp ExtractBoardList.h
		$(CXX) -c $< $(CPPFLAGS)
JaneCloneFrame.o : JaneClone.cpp JaneClone.h　ExtractBoardList.h SocketCommunication.h DataType.h SQLiteBundle.h
		$(CXX) -c $< $(CPPFLAGS)
Main.o : Main.cpp JaneClone.h
		$(CXX) -c $< $(CPPFLAGS)
.PHONY: clean
clean:
	$(RM) -f *.o *.exe
