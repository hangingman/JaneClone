################################################################################
# JaneCloneビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = JaneClone
SOURCES = $(notdir $(shell find src/ -name '*.cpp'))
RCS		= $(notdir $(shell find rc/ -name '*.rc'))
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS +=$(RCS:.rc=.o)
# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
DEP	= dep

# depend library
LIBNKF	= libwxnkf.a
NKFDIR	= libwxnkf

# basic command
CXX	:= g++
RM 	:= rm
WINDRES := windres
MAKE	:= make

# debug and release
CXX_DEBUG_FLAGS		=	-gstabs
CXX_RELEASE_FLAGS	=	-s

# compile option
CXXFLAGS = -Wall -I/c/MinGW/include -I include -I libwxnkf/include `wx-config --cxxflags` `xml2-config --cflags` -I$(NKFDIR)/src
RCFLAGS  = $(shell wx-config --cxxflags | awk '{ print $$2 }')  # wx-configからwxのヘッダファイルのルートディレクトリを取り出している
LDFLAGS  = -static -L/c/MinGW/lib -lwx_mswu_aui-2.9 `wx-config --libs` `xml2-config --libs` $(NKFDIR)/libwxnkf.a -lmk4
VPATH    = src rc

# dummy target
.PHONY	: Debug Release clean all-clean

# make all
all : $(LIBNKF) $(DEP) $(TARGET)
$(TARGET): $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)

# suffix rule
.SUFFIXES: .cpp .o
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<
# make dependency
dep:
	$(CXX) -MM -MG $(DEPSRCS) >makefile.dep
# debug
Debug 	: CXX+=$(CXX_DEBUG_FLAGS)
Debug 	: all
# release
Release	: CXX+=$(CXX_RELEASE_FLAGS)
Release	: all

# icon build
icon.o : icon.rc
		$(WINDRES) -i $^ -O coff -o $@  $(RCFLAGS)

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
	$(RM) -f *.o $(TARGET).exe makefile.dep
# all-clean
all-clean:
	$(RM) -f *.o $(TARGET).exe makefile.dep
	$(MAKE) -C $(NKFDIR) clean