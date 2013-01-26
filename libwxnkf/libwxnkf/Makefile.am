ACLOCAL_AMFLAGS = -I m4
lib_LTLIBRARIES = libwxnkf.la
# Sources for libwxnkf
libwxnkf_la_SOURCES = flagset.cpp   \
					  utf8table.cpp \
					  utf16util.cpp \
					  util.cpp      \
					  wxnkf.cpp     \
					  wxnkfencoding.cpp
# Linker options
libwxnkf_la_LDFLAGS=
libwxnkf_la_LIBADD=@LIBS@
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libwxnkf_la_CXXFLAGS = -I../include @CXXFLAGS@
libwxnkf_la_CPPFLAGS = -I../include @CPPFLAGS@

