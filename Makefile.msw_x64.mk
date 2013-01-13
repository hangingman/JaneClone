################################################################################
# JaneCloneビルド用Makefile(x64) 
# Contributor:
#	Hiroyuki Nagata <newserver002@gmail.com>
################################################################################

# target and sources
TARGET  = JaneClone
SOURCES = $(notdir $(shell find src/ -name '*.cpp'))
RCS	= $(notdir $(shell find rc/ -name '*.rc'))
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS +=$(RCS:.rc=.o)
# sources for dependency
DEPSRCS = $(shell find src/ -name '*.cpp')
DEP	= dep

# depend library
LIBNKF	= libwxnkf.a
NKFDIR	= libwxnkf

# basic command
SYSROOT = /c/Mingw64
CXX	= $(SYSROOT)/bin/x86_64-w64-mingw32-g++
RM 	= rm
WINDRES = $(SYSROOT)/bin/windres
MAKE	:= make
WX-CXXFLAGS =  $(shell $(SYSROOT)/bin/wx-config --version=2.9 --static=yes --cxxflags | sed -e s'/\/mingw/\/c\/MinGW64/g')
WX-LDFLAGS  = $(shell $(SYSROOT)/bin/wx-config --version=2.9 --static=yes --libs aui,qa,adv,base,core,html,net | sed -e s'/\/mingw/\/c\/MinGW64/g')
XML2-CFLAGS = $(shell $(SYSROOT)/bin/xml2-config --cflags | sed -e s'/\/mingw/\/c\/MinGW64/g')
XML2-LDFLAGS = $(shell $(SYSROOT)/bin/xml2-config --libs | sed -e s'/\/mingw/\/c\/MinGW64/g')

# debug and release
CXX_DEBUG_FLAGS		=	-gstabs
CXX_RELEASE_FLAGS	=	-s -O2

# compile option
CXXFLAGS = -Wall -DWX_PRECOMP -mthreads -fno-strict-aliasing -I include -I libwxnkf/include $(WX-CXXFLAGS) $(XML2-CFLAGS) -I$(NKFDIR)/src
LDFLAGS  = $(WX-LDFLAGS) $(XML2-LDFLAGS) $(NKFDIR)/libwxnkf.a -lmk4
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
		$(WINDRES) -i $^ -O coff -o $@ $(shell echo $(WX-CXXFLAGS) | awk '{print $$2}')

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