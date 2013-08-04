janecloneexecdir 	= $(datarootdir)/@PACKAGE@
janecloneexec_PROGRAMS 	= JaneClone
janecloneexecrcdir	= $(datarootdir)/@PACKAGE@/rc
janecloneexecrc_DATA	= $(wildcard rc/*)

JaneClonedir	  = $(pkglibdir)
JaneClone_SOURCES = extractboardlist.hpp        \
		    main.cpp                    \
		    socketcommunication.cpp     \
		    threadcontentwindow.hpp     \
		    anchoredresponsepopup.cpp   \
		    janeclone.cpp               \
		    sqliteaccessor.cpp          \
		    socketcommunication.hpp     \
		    virtualboardlistctrl.cpp    \
		    anchoredresponsepopup.hpp   \
		    janeclone.hpp               \
		    sqliteaccessor.hpp          \
		    threadcontentbar.cpp        \
		    virtualboardlistctrl.hpp    \
		    datatype.hpp                \
		    janecloneutil.cpp           \
		    responsewindow.cpp          \
		    threadcontentbar.hpp        \
		    extractboardlist.cpp        \
		    janecloneutil.hpp           \
		    responsewindow.hpp          \
		    threadcontentwindow.cpp	\
		    janecloneimageviewer.cpp	\
		    janecloneimageviewer.hpp    \
		    imagepanel.cpp		\
		    imagepanel.hpp		\
		    settingwindow.cpp		\
		    settingwindow.hpp		\
		    otherpanels.cpp		\
		    otherpanels.hpp		\
		    enums.hpp			\
		    enumstring.hpp              \
		    $(wildcard rc/*)

# common build rule
JaneClone_CXXFLAGS = @CXXFLAGS@ @XML2CFLAGS@ -I$(top_builddir)/libwxnkf -I$(top_builddir)/wxsqlite3 -I$(top_builddir)/sqlite3 -I$(top_builddir)/src/rc -I$(top_builddir)/libwxuuid -I$(top_builddir)/curl/include -I$(top_builddir)/curlpp/include
ACLOCAL_AMFLAGS    = -I m4

# For Icon build and library dependency
if USE_WINDRES
# Platform is Windows
JaneClone_LDADD    = JaneClone_rc.o $(top_builddir)/libwxnkf/libwxnkf.la $(top_builddir)/libwxuuid/libwxuuid.la $(top_builddir)/wxsqlite3/libwxsqlite3.la $(top_builddir)/sqlite3/libsqlite3.la $(top_builddir)/curl/lib/libcurl.la $(top_builddir)/curlpp/src/curlpp/libcurlpp.la $(top_builddir)/curlpp/src/utilspp/libutilspp.la $(top_builddir)/curlpp/src/curlpp/internal/libinternal.la

JaneClone_LDFLAGS  = `@WX_CONFIG_PATH@ --libs aui,qa,adv,base,core,html,net` @XML2LIBS@ JaneClone_rc.o

JaneClone_rc.o : $(top_builddir)/src/rc/icon.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@

else
# Platform is not Windows
JaneClone_LDFLAGS  = `@WX_CONFIG_PATH@ --libs aui,qa,adv,base,core,html,net` @XML2LIBS@ -L$(top_builddir)/libwxnkf -lwxnkf -L$(top_builddir)/libwxuuid -lwxuuid -L$(top_builddir)/wxsqlite3 -lwxsqlite3 -L$(top_builddir)/sqlite3 -lsqlite3 -L$(top_builddir)/curl/lib -lcurl -L$(top_builddir)/curlpp/src/curlpp -lcurlpp -L$(top_builddir)/curlpp/src/utilspp -lutilspp -L$(top_builddir)/curlpp/src/curlpp/internal -linternal 
endif

# target for debug