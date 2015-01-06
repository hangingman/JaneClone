AM_CFLAGS 		= -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_RTREE
ACLOCAL_AMFLAGS 	= -I m4

lib_LTLIBRARIES		= libsqlite3.la
libsqlite3_la_SOURCES   = sqlite3.c
libsqlite3_la_LDFLAGS   = -no-undefined -version-info 8:6:8

noinst_HEADERS = sqlite3.h sqlite3ext.h
