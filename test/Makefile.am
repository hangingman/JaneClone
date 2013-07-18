check_PROGRAMS		= jc-test1
TESTS			= jc-test1
AUTOMAKE_OPTIONS	= color-tests

jc_test1_SOURCES	= jc-test1.cpp
jc_test1_CXXFLAGS	= @CXXFLAGS@ -I$(top_builddir)/libwxnkf
jc_test1_LDADD		= $(top_builddir)/libwxnkf/libwxnkf.la
jc_test1_LDFLAGS	= `@WX_CONFIG_PATH@ --libs base,core`