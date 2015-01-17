AM_CXXFLAGS=

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES		= libwxsqlite3.la
libwxsqlite3_la_SOURCES = wxsqlite3.cpp
libwxsqlite3_la_LDFLAGS = -no-undefined -version-info 3:0:2
libwxsqlite3_la_LIBADD	=

if BUILDING_SQLITE3
  AM_CXXFLAGS            += -I$(top_builddir)/sqlite3 -DWXMAKINGDLL_WXSQLITE3 -DwxUSE_DYNAMIC_SQLITE3_LOAD=0 -DWXSQLITE3_HAVE_METADATA=0 -DWXSQLITE3_HAVE_CODEC=0 -DWXSQLITE3_HAVE_LOAD_EXTENSION=0
  libwxsqlite3_la_LIBADD += $(top_builddir)/sqlite3/libsqlite3.la
endif

noinst_HEADERS = \
	wx/wxsqlite3.h    \
	wx/wxsqlite3def.h \
	wx/wxsqlite3dyn.h \
	wx/wxsqlite3opt.h
