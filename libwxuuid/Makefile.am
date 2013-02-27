noinst_LIBRARIES = libwxuuid.a
# Sources for libwxnkf
libwxuuid_a_SOURCES = wxUUID.cpp \
		      wxMD5.cpp  \
		      wxGUID.cpp \
		      md5c.c
# Linker options
libwxuuid_a_LDFLAG = -static
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libwxuuid_a_CXXFLAGS = -I$(top_builddir)/libwxuuid @CXXFLAGS@
ACLOCAL_AMFLAGS = -I m4
