AM_CXXFLAGS = -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_RTREE -I$(top_builddir)/sqlite3
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libwxsqlite3.la
libwxsqlite3_la_SOURCES = wxsqlite3.cpp
libwxsqlite3_la_LDFLAGS = -no-undefined -version-info 3:0:2
libwxsqlite3_la_LIBADD	= $(top_builddir)/sqlite3/libsqlite3.la