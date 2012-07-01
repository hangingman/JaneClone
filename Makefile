################################################################################
# JaneCloneビルド用Makefile 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = JaneClone
SOURCES = $(notdir $(shell find . -name '*.cpp'))
RCS		= $(notdir $(shell find . -name '*.rc'))
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS +=$(RCS:.rc=.o)

# depend library
LIBNKF	= libnkf.a
NKFDIR	= libnkf

# basic command
CXX		:= g++
RM 		:= rm
WINDRES := windres
MAKE	:= make

# debug and release
CXX_DEBUG_FLAGS		=	-gstabs
CXX_RELEASE_FLAGS	=	-s

# compile option
CXXFLAGS = -Wall -I/c/MinGW/include -I include `wx-config --cxxflags` `xml2-config --cflags` -I$(NKFDIR)
RCFLAGS  = $(shell wx-config --cxxflags | awk '{ print $$2 }')  # wx-configからwxのヘッダファイルのルートディレクトリを取り出している
LDFLAGS  = -static -L/c/MinGW/lib -lwx_mswu_aui-2.9 `wx-config --libs` `xml2-config --libs` $(NKFDIR)/libnkf.a -lmk4
VPATH    = include src rc libnkf

# dummy target
.PHONY	: Debug Release clean all-clean

# make all
all : $(LIBNKF) $(TARGET)
$(TARGET): $(OBJECTS)
		$(CXX) $^ -o $@ $(LDFLAGS)

# suffix rule
.SUFFIXES: .cpp .o
.cpp.o:
		$(CXX) $(CXXFLAGS) -c $<

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
	@echo "libnkf build"
	@if [ -f $(NKFDIR)/$(LIBNKF) ]; then \
		echo "libnkf found"; \
	else \
		echo "libnkf not found, so now build"; \
		$(MAKE) -C $(NKFDIR) ; \
	fi

# clean
clean:
	$(RM) -f *.o $(TARGET).exe
# all-clean
all-clean:
	$(RM) -f *.o $(TARGET).exe
	$(MAKE) -C $(NKFDIR) clean