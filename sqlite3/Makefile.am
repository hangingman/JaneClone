noinst_LIBRARIES = libsqlite3.a
# Sources for libsqlite3
libsqlite3_a_SOURCES = shell.c sqlite3.c
# Linker options
libsqlite3_a_LDFLAG = -static -s
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libsqlite3_a_CXXFLAGS = -I$(top_builddir)/sqlite3 @CXXFLAGS@
ACLOCAL_AMFLAGS = -I m4
