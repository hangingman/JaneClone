ACLOCAL_AMFLAGS		= -I m4

janeclonedir 		= $(pkglibdir)
janeclone_LTLIBRARIES	= libwxuuid.la
libwxuuid_la_SOURCES	= wxUUID.cpp wxMD5.cpp wxGUID.cpp md5c.c
libwxuuid_la_LDFLAGS 	= -no-undefined -version-info 1:0:0

noinst_HEADERS = \
	global.h  \
	md5.h     \
	wxGUID.h  \
	wxMD5.h   \
	wxUUID.h

if USE_WINDRES
libwxuuid_la_LDFLAGS 	+= -loleaut32 -lole32
endif 