AUTOMAKE_OPTIONS 	= foreign subdir-objects
EXTRA_DIST		= rc
EXTRA_DATA		= $(wildcard rc/*)

janecloneexecdir 	= $(datarootdir)/@PACKAGE@
janecloneexec_PROGRAMS 	= JaneClone

janecloneexecrcdir	= $(datarootdir)/@PACKAGE@/rc
janecloneexecrc_DATA	= $(wildcard rc/*)

janecloneexecscriptdir	= $(datarootdir)/@PACKAGE@/script
janecloneexecscript_DATA= $(wildcard script/*)

JaneClonedir	  	= $(pkglibdir)
JaneClone_SOURCES 	= extractboardlist.hpp        \
		          main.cpp                    \
		          socketcommunication.cpp     \
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
		          janecloneimageviewer.cpp    \
		          janecloneimageviewer.hpp    \
		          imagepanel.cpp	      \
			  imagepanel.hpp	      \
			  settingwindow.cpp	      \
			  settingwindow.hpp	      \
			  otherpanels.cpp	      \
			  otherpanels.hpp	      \
		          drawpane.cpp                \
		          drawpane.hpp                \
		          enums.hpp		      \
		          enumstring.hpp              \
		          janecloneuiutil.cpp         \
		          janecloneuiutil.hpp         \
			  ../babel/babel.cpp          \
			  ../babel/babel.h            \
			  ../json11/json11.cpp	      \
			  ../json11/json11.hpp

if USE_WX_WEBVIEW
# wxWebView support
JaneClone_SOURCES += threadcontentwebview.cpp   \
		     threadcontentwebview.hpp

JC_FLAG_STRING	   = `@WX_CONFIG_PATH@ --version=@WX_VERSION_MAJOR@.@WX_VERSION_MINOR@ --libs aui,qa,adv,base,core,html,net,xrc,webview`

# javascript
EXTRA_DIST		+= script
EXTRA_SCRIPTS		= $(wildcard script/*)

else
# Use wxHtmlWindow
JaneClone_SOURCES += threadcontentwindow.cpp    \
		     threadcontentwindow.hpp

JC_FLAG_STRING	   = `@WX_CONFIG_PATH@ --version=@WX_VERSION_MAJOR@.@WX_VERSION_MINOR@ --libs aui,qa,adv,base,core,html,net,xrc`
endif

# common build rule
JaneClone_CXXFLAGS = @CXXFLAGS@ @XML2CFLAGS@          \
		     -I$(top_builddir)/crossguid      \
		     -I$(top_builddir)/babel          \
		     -I$(top_builddir)/wxsqlite3      \
		     -I$(top_builddir)/sqlite3        \
		     -I$(top_builddir)/src/rc         \
		     -I$(top_builddir)/curlpp/include

ACLOCAL_AMFLAGS    = -I m4

# For Icon build and library dependency
if USE_WINDRES
# Platform is Windows
JaneClone_LDADD    = JaneClone_rc.o                                           \
		    $(top_builddir)/crossguid/libcrossguid.la		      \
		    $(top_builddir)/wxsqlite3/libwxsqlite3.la 		      \
		    $(top_builddir)/sqlite3/libsqlite3.la 		      \
		    $(top_builddir)/curlpp/src/curlpp/libcurlpp.la 	      \
		    $(top_builddir)/curlpp/src/utilspp/libutilspp.la 	      \
		    $(top_builddir)/curlpp/src/curlpp/internal/libinternal.la

JaneClone_LDFLAGS  = $(JC_FLAG_STRING) @XML2LIBS@ JaneClone_rc.o

JaneClone_rc.o : $(top_builddir)/src/rc/icon.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@

else
# Platform is not Windows
JaneClone_LDFLAGS  = $(JC_FLAG_STRING)                                          \
		    @XML2LIBS@							\
		    -L$(top_builddir)/crossguid			 -lcrossguid	\
		    -L$(top_builddir)/wxsqlite3			 -lwxsqlite3	\
		    -L$(top_builddir)/sqlite3			 -lsqlite3	\
		    -L$(top_builddir)/curlpp/src/curlpp		 -lcurlpp	\
		    -L$(top_builddir)/curlpp/src/utilspp	 -lutilspp	\
		    -L$(top_builddir)/curlpp/src/curlpp/internal -linternal
endif

# target for debug
