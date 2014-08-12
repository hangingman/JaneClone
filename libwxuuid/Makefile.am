ACLOCAL_AMFLAGS		= -I m4
AUTOMAKE_OPTIONS	= serial-tests subdir-objects

janeclonedir 		= $(pkglibdir)
janeclone_LTLIBRARIES	= libwxuuid.la
libwxuuid_la_SOURCES	= wxUUID.cpp wxMD5.cpp wxGUID.cpp md5c.c
libwxuuid_la_LDFLAGS 	= -no-undefined -version-info 1:0:0

noinst_HEADERS = \
	global.h  \
	md5.h     \
	wxGUID.h  \
	wxMD5.h   \
	wxUUID.h

if USE_WINDRES
libwxuuid_la_LDFLAGS 	+= -loleaut32 -lole32
endif

### Unit Test ###
check_PROGRAMS		= wxuuid-test1
TEST_LOGS		= wxuuid-test1.log
wxuuid_test1_SOURCES	= test/wxuuid-test1.cpp
wxuuid_test1_LDFLAGS	= -L$(top_builddir)/libwxuuid -lwxuuid
wxuuid_test1_CXXFLAGS	= -Dprotected=public

check-local:
	@for f in $(TEST_LOGS); do printf "\n=== $$f ===\n\n"; cat $$f; done
