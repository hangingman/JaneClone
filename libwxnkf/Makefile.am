ACLOCAL_AMFLAGS = -I m4

janeclonedir 		= $(pkglibdir)
janeclone_LTLIBRARIES	= libwxnkf.la
AM_CXXFLAGS		= -I$(top_builddir)/libwxnkf
libwxnkf_la_SOURCES	= flagset.cpp utf8table.cpp utf16util.cpp util.cpp wxnkf.cpp wxnkfencoding.cpp
libwxnkf_la_LDFLAGS 	= -no-undefined -version-info 1:0:0

noinst_HEADERS = \
	flagset.h         \
	utf16util.h       \
	utf8table.h       \
	util.h            \
	wxnkfbaseconfig.h \
	wxnkfencoding.h   \
	wx/wxnkf.h
