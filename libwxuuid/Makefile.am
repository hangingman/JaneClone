ACLOCAL_AMFLAGS		= -I m4

lib_LTLIBRARIES 	= libwxuuid.la
libwxuuid_la_SOURCES	= wxUUID.cpp wxMD5.cpp wxGUID.cpp md5c.c
libwxuuid_la_LDFLAGS 	= -no-undefined -version-info 1:0:0

if USE_WINDRES
libwxuuid_la_LDFLAGS 	+= -loleaut32 -lole32
endif 