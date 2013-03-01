noinst_LIBRARIES = libwxsqlite3.a
# Sources for libsqlite3
libwxsqlite3_a_SOURCES = wxsqlite3.cpp
# Linker options
libwxsqlite3_a_LDFLAG = -static -s
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libwxsqlite3_a_CXXFLAGS = -I$(top_builddir)/sqlite3 -I$(top_builddir)/wxsqlite3 @CXXFLAGS@
ACLOCAL_AMFLAGS = -I m4
