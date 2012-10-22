################################################################################
# libwxnkfビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = libwxnkf.a
SOURCES = $(shell find src/ -name '*.cpp')
OBJECTS = $(notdir $(SOURCES:.cpp=.o))

# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
DEP	= dep

# basic command
CXX	:= g++ -O2 -s
AR      := ar
RM	:= rm
MV	:= mv
VERSION := 2.1.2
MKDIR	:= mkdir
MAKE	:= make

# compile option
CXXFLAGS = -Wall `wx-config --cxxflags` -I src
LDFLAGS := -static `wx-config --libs`
ARFLAG	:= crsv
VPATH	= src

# dummy target
.PHONY: dep clean
# make all
all:	$(DEP) $(TARGET)
# suffix rule
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
# make dependency
$(DEP):
	$(CXX) -MM -MG $(DEPSRCS) >makefile.dep
# build library
$(TARGET) : $(OBJECTS)
		$(AR) $(ARFLAG) $(TARGET) $(OBJECTS)
# clean
clean:
		$(RM) -f *.o $(TARGET) makefile.dep
# dependency
-include makefile.dep