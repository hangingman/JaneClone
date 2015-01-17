AUTOMAKE_OPTIONS 	= foreign subdir-objects
EXTRA_DIST		= rc
EXTRA_DATA		= $(wildcard rc/*)

bin_PROGRAMS 		= janeclone

janecloneexecrcdir	= $(prefix)/share/pixmaps/@PACKAGE@
janecloneexecrc_DATA	= $(wildcard rc/*)

janecloneexecscriptdir	= $(datarootdir)/@PACKAGE@/script
janecloneexecscript_DATA= $(wildcard script/*)

janeclone_SOURCES 	= extractboardlist.hpp        \
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
janeclone_SOURCES += threadcontentwebview.cpp   \
		     threadcontentwebview.hpp

JC_FLAG_STRING	   = `@WX_CONFIG_PATH@ --version=@WX_VERSION_MAJOR@.@WX_VERSION_MINOR@ --libs aui,qa,adv,base,core,html,net,xrc,webview`

# javascript
EXTRA_DIST		+= script
EXTRA_SCRIPTS		= $(wildcard script/*)

else
# Use wxHtmlWindow
janeclone_SOURCES += threadcontentwindow.cpp    \
		     threadcontentwindow.hpp

JC_FLAG_STRING	   = `@WX_CONFIG_PATH@ --version=@WX_VERSION_MAJOR@.@WX_VERSION_MINOR@ --libs aui,qa,adv,base,core,html,net,xrc`
endif

# common build rule
janeclone_CXXFLAGS = @CXXFLAGS@ @XML2CFLAGS@ @CRYPTO_CFLAGS@ \
		     -I$(top_builddir)/babel          \
		     -I$(top_builddir)/src/rc

ACLOCAL_AMFLAGS    = -I m4

# For Icon build and library dependency
if USE_WINDRES
# Platform is Windows
janeclone_LDFLAGS  = $(JC_FLAG_STRING) janeclone_rc.o
janeclone_LDADD    = @XML2LIBS@ @CRYPTO_LIBS@ $(top_builddir)/cmdline_twitter/libctw.a

janeclone_rc.o : $(top_builddir)/src/rc/icon.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@

else
# Platform is not Windows
janeclone_LDFLAGS  = $(JC_FLAG_STRING)
janeclone_LDADD    = @XML2LIBS@ @CRYPTO_LIBS@ $(top_builddir)/cmdline_twitter/libctw.a

endif

# Build or not
if BUILDING_SQLITE3
  janeclone_CXXFLAGS += -I$(top_builddir)/sqlite3
  janeclone_LDADD    += $(top_builddir)/sqlite3/libsqlite3.a
endif

if BUILDING_WXSQLITE3
  janeclone_CXXFLAGS += -I$(top_builddir)/wxsqlite3
  janeclone_LDADD    += $(top_builddir)/wxsqlite3/libwxsqlite3.a
endif

if BUILDING_CURLPP
  janeclone_CXXFLAGS += -I$(top_builddir)/curlpp/include
  janeclone_LDADD    += $(top_builddir)/curlpp/src/curlpp/libcurlpp.la 	          \
		        $(top_builddir)/curlpp/src/utilspp/libutilspp.la 	  \
		        $(top_builddir)/curlpp/src/curlpp/internal/libinternal.la
endif
