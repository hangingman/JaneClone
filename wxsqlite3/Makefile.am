AM_CXXFLAGS=

ACLOCAL_AMFLAGS = -I m4

noinst_LIBRARIES	= libwxsqlite3.a
libwxsqlite3_a_SOURCES  = wxsqlite3.cpp
#libwxsqlite3_a_LDFLAGS  = -no-undefined -version-info 3:0:2
libwxsqlite3_a_LIBADD	=

if BUILDING_SQLITE3
  # -DWXMAKINGDLL_WXSQLITE3
  AM_CXXFLAGS            += -I$(top_builddir)/sqlite3 -DwxUSE_DYNAMIC_SQLITE3_LOAD=0 -DWXSQLITE3_HAVE_METADATA=0 -DWXSQLITE3_HAVE_CODEC=0 -DWXSQLITE3_HAVE_LOAD_EXTENSION=0
  libwxsqlite3_a_LIBADD += $(top_builddir)/sqlite3/libsqlite3.a
endif

noinst_HEADERS = \
	wx/wxsqlite3.h    \
	wx/wxsqlite3def.h \
	wx/wxsqlite3dyn.h \
	wx/wxsqlite3opt.h
