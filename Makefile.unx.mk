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
CXX_RELEASE_FLAGS = -O2

# compile option
PREFIX	  = /usr/local/
CXXFLAGS = -Wall -I$(PREFIX)include -I include \
`wx-config --cxxflags` `xml2-config --cflags`   \
-I$(NKFDIR)/include

# 足りない静的ライブラリを補充して並び替えた
LDFLAGS  = -static									\
-L/usr/local/lib	-L$(NKFDIR)/					\
-lwx_gtk2u_aui-2.9 \
/usr/local/lib/libwx_gtk2u_xrc-2.9.a 				\
/usr/local/lib/libwx_gtk2u_qa-2.9.a 				\
/usr/local/lib/libwx_baseu_net-2.9.a 				\
/usr/local/lib/libwx_gtk2u_html-2.9.a 			\
/usr/local/lib/libwx_gtk2u_adv-2.9.a 				\
/usr/local/lib/libwx_gtk2u_core-2.9.a 			\
/usr/local/lib/libwx_baseu_xml-2.9.a 				\
-lwx_baseu-2.9					\
-lgtk-x11-2.0 -lgdk-x11-2.0							\
-lwxregexu-2.9 -lwxtiff-2.9 -lwxjpeg-2.9			\
-latk-1.0 -lXfixes -lpangoft2-1.0 -lXext -lXi -lXdamage -lXcomposite -lXrandr -lXcursor -lXinerama	\
-lgdk_pixbuf-2.0 -ljasper -ljpeg -lm -lpangocairo-1.0	\
-lcairo	-lXrender -lpixman-1 -lgio-2.0 -lselinux	\
-lpango-1.0 -lthai -ldatrie -lgobject-2.0 -lgmodule-2.0  \
-lgthread-2.0 -lrt -lpng -lz -lxml2 -lwxnkf -lmk4 \
-lfontconfig -lfreetype -lexpat -lssl -lcrypto    \
-lglib-2.0 -lpcre -lX11 -lxcb -lXdmcp -lXau       \
-lSM -lICE -lpthread -ldl

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