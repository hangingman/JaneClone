################################################################################
# libwxnkfビルド用Makefile(x64)
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
SYSROOT = /c/MinGW64
CXX	= $(SYSROOT)/bin/x86_64-w64-mingw32-g++ -O2 -s
AR      = $(SYSROOT)/x86_64-w64-mingw32/bin/ar
RM	= rm
MV	= mv
VERSION = 2.1.2
MKDIR	= mkdir
MAKE	= make
WX-CXXFLAGS =  $(shell $(SYSROOT)/bin/wx-config --version=2.9 --static=yes --cxxflags | sed -e s'/\/mingw/\/c\/MinGW64/g')
WX-LDFLAGS  = $(shell $(SYSROOT)/bin/wx-config --version=2.9 --static=yes --libs | sed -e s'/\/mingw/\/c\/MinGW64/g')

# compile option
CXXFLAGS = -Wall $(WX-CXXFLAGS) -I src
LDFLAGS  = $(WX-LDFLAGS)
ARFLAG	 = crsv
VPATH	 = src

# dummy target
.PHONY: dep clean test
# make all
all:	$(DEP) $(TARGET)
# suffix rule
.cpp.o:
		@echo $(CXX) $(CXXFLAGS) -c $<
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