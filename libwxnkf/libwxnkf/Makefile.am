ACLOCAL_AMFLAGS = -I m4
noinst_LIBRARIES = libwxnkf.a
# Sources for libwxnkf
libwxnkf_a_SOURCES = flagset.cpp   \
	   	      utf8table.cpp \
		      utf16util.cpp \
		      util.cpp      \
		      wxnkf.cpp     \
		      wxnkfencoding.cpp
# Linker options
libwxnkf_a_LDFLAG = -static
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libwxnkf_a_CXXFLAGS = -I$(top_builddir)/libwxnkf/include @CXXFLAGS@
libwxnkf_a_CPPFLAGS = -I$(top_builddir)/libwxnkf/include @CPPFLAGS@

