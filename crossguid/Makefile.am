AM_CXXFLAGS		= @CXXFLAGS@
libcrossguid_la_LDFLAGS =

if USE_WINDRES
AM_CXXFLAGS 	        +=  -DGUID_WINDOWS
libcrossguid_la_LDFLAGS += -lole32
endif

if USE_MAC_OSX
AM_CXXFLAGS 	+=  -DGUID_CFUUID
endif

if USE_LINUX
AM_CXXFLAGS 	+=  -DGUID_LIBUUID
endif

ACLOCAL_AMFLAGS = -I m4

janeclonedir 		= $(pkglibdir)
janeclone_LTLIBRARIES	= libcrossguid.la
libcrossguid_la_SOURCES = guid.cpp guid.h
libcrossguid_la_LDFLAGS += -no-undefined -version-info 1:0:0

noinst_HEADERS = guid.h
