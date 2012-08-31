################################################################################
#
# JaneCloneビルド用Makefile 
# Copyright (c) 2012 HIROYUKI Nagata <jagdironscrap@gmail.com>
#
################################################################################

# target and sources
TARGET  = JaneClone
SOURCES = $(notdir $(shell find src/ -name '*.cpp'))
OBJECTS = $(SOURCES:.cpp=.o)

# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
DEP		= dep

# depend library
LIBNKF	= libwxnkf.a
NKFDIR	= libwxnkf

# basic command
CXX		:= g++
RM 		:= rm
MAKE	:= make

# debug and release
CXX_DEBUG_FLAGS   =	-gstabs
CXX_RELEASE_FLAGS = -O2 -s

# compile option
CXXFLAGS = -Wall -I/usr/local/include -I include `wx-config --cxxflags` `xml2-config --cflags` -I$(NKFDIR)/include
LDFLAGS  = -lwx_gtk2u_aui-2.9 `wx-config --libs` `xml2-config --libs` -lmk4 $(NKFDIR)/libwxnkf.a
VPATH    = include src rc

# dummy target
.PHONY	: Debug Release clean all-clean

# make all
all : $(LIBNKF) $(DEP) $(TARGET)
$(TARGET): $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)
# make dependency
dep:
	$(CXX) -MM -MG $(DEPSRCS) >makefile.dep
# suffix rule
.SUFFIXES: .cpp .o
.cpp.o:
		$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

# debug
Debug 	: CXX+=$(CXX_DEBUG_FLAGS)
Debug 	: all
# release
Release	: CXX+=$(CXX_RELEASE_FLAGS)
Release	: all

# libnkf build
$(LIBNKF):
	@echo "libwxnkf build"
	@if [ -f $(NKFDIR)/$(LIBNKF) ]; then \
		echo "libwxnkf found"; \
	else \
		echo "libwxnkf not found, so now build"; \
		$(MAKE) -C $(NKFDIR) ; \
	fi

# clean
clean:
	$(RM) -f *.o $(TARGET)
# all-clean
all-clean:
	$(RM) -f *.o $(TARGET) makefile.dep
	$(MAKE) -C $(NKFDIR) clean