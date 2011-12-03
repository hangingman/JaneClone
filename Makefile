TARGET 	= JaneClone
OBJECTS = Main.o JaneClone.o

# 基本コマンド
RM 		:= rm
CXX 	:= g++
CC 		:= g++

# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-g -O0
CXX_RELEASE_FLAGS	=	-s -O2

# オプション
CPPFLAGS = -Wall -I/c/MinGW/include `wx-config --cxxflags` -I include -I/c/Boost
LDFLAGS  = -L/c/MinGW/lib `wx-config --libs` \
-lws2_32 \
-lz \
-lboost_filesystem-mgw45-mt-1_47 \
-lboost_iostreams-mgw45-mt-1_47 \
-lboost_system-mgw45-mt-1_47 \

VPATH    = include src

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
		$(CXX) -c $< $(CPPFLAGS)
JaneCloneFrame.o : JaneClone.cpp JaneClone.h
		$(CXX) -c $< $(CPPFLAGS)
Main.o : Main.cpp JaneClone.h
		$(CXX) -c $< $(CPPFLAGS)
.PHONY: clean
clean:
	$(RM) -f *.o *.exe
