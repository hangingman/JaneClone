bin_PROGRAMS      = JaneClone
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
		    janecloneimageviewer.hpp

# common build rule
JaneClone_CXXFLAGS = @CXXFLAGS@ @XML2CFLAGS@ -I$(top_builddir)/libwxnkf -I$(top_builddir)/wxsqlite3 -I$(top_builddir)/sqlite3 -I$(top_builddir)/rc -I$(top_builddir)/libwxuuid
ACLOCAL_AMFLAGS    = -I m4

# For Icon build and library dependency
if USE_WINDRES
# Platform is Windows
JaneClone_LDADD    = $(top_builddir)/libwxnkf/libwxnkf.a $(top_builddir)/libwxuuid/libwxuuid.a $(top_builddir)/wxsqlite3/libwxsqlite3.a $(top_builddir)/sqlite3/libsqlite3.a JaneClone_rc.o
JaneClone_LDFLAGS  = -static `wx-config --libs aui,qa,adv,base,core,html,net` @XML2LIBS@

JaneClone_rc.o : $(top_builddir)/rc/icon.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@

else
# Platform is not Windows
JaneClone_LDADD    = $(top_builddir)/libwxnkf/libwxnkf.a $(top_builddir)/libwxuuid/libwxuuid.a $(top_builddir)/wxsqlite3/libwxsqlite3.a $(top_builddir)/sqlite3/libsqlite3.a
JaneClone_LDFLAGS  = `wx-config --libs aui,qa,adv,base,core,html,net` @XML2LIBS@
endif

################################ install target for JaneClone ###############################

packagedir	= package
rcdir		= $(top_builddir)/rc

################################ setting for Mac OSX ########################################

OUTPUTPATH	= $(top_builddir)
PROGRAM		= $(bin_PROGRAMS)
PROGVER		= 0.7
ICONPATH	= $(rcdir)/janeclone.icns
ICON		= janeclone.icns

# Info.plist
COMPANY		= Hiroyuki Nagata
BUNDLEPACKAGE	= APPL
BUNDLESIGNATURE	= ????

# application bundle
BUNDLE		=$(OUTPUTPATH)/$(PROGRAM).app
MACPKGINFO	=$(BUNDLE)/Contents/PkgInfo
MACINFOPLIST=$(BUNDLE)/Contents/Info.plist

################################ install script #############################################

install : 
	@echo "start janeclone install..."

	@if [ -f $(top_builddir)/$(packagedir) ]; then \
		rm -rf $(top_builddir)/$(packagedir); \
	fi
	$(MKDIR_P) $(top_builddir)/$(packagedir)
#
# install target for Win32
#
	@if test "@PLATFORM_STRING@" = "Win32"; then \
		echo "platform is Win32"; \
		cp -rp $(rcdir) $(top_builddir)/$(packagedir); \
		cp -p $(bin_PROGRAMS) $(top_builddir)/$(packagedir); \
	fi
#
# install target for Linux
#
	@if test "@PLATFORM_STRING@" = "Linux"; then \
		echo "platform is Linux"; \
		cp -rp $(rcdir) $(top_builddir)/$(packagedir); \
		cp -p $(bin_PROGRAMS) $(top_builddir)/$(packagedir); \
	fi
#
# install target for MacOSX
#
	@if test "@PLATFORM_STRING@" = "Apple"; then \
		echo "platform is Apple"; \
		cp -rp $(rcdir) $(top_builddir)/$(packagedir); \
		$(MKDIR_P) $(OUTPUTPATH);			\
		$(MKDIR_P) $(BUNDLE)/Contents;			\
		$(MKDIR_P) $(BUNDLE)/Contents/MacOS;		\
		$(MKDIR_P) $(BUNDLE)/Contents/Resources;	\
		touch $(MACPKGINFO);						\
		echo "$(BUNDLEPACKAGE)$(BUNDLESIGNATURE)" > $(MACPKGINFO);	\
		touch $(MACINFOPLIST);										\
		echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> $(MACINFOPLIST);				\
		echo "<!DOCTYPE plist PUBLIC " >> $(MACINFOPLIST);						\
		echo "\"-//Apple Computer//DTD PLIST 1.0//EN\" " >> $(MACINFOPLIST);				\
		echo "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $(MACINFOPLIST);			\
		echo "<plist version=\"1.0\">" >> $(MACINFOPLIST);						\
		echo "<dict>" >> $(MACINFOPLIST);								\
		echo "   <key>CFBundleDevelopmentRegion</key>" >> $(MACINFOPLIST);				\
		echo "   <string>English</string>" >> $(MACINFOPLIST);						\
		echo "   <key>CFBundleExecutable</key>" >> $(MACINFOPLIST);					\
		echo "   <string>$(PROGRAM)</string>" >> $(MACINFOPLIST);					\
		echo "   <key>CFBundleIconFile</key>" >> $(MACINFOPLIST);					\
		echo "   <string>$(ICON)</string>" >> $(MACINFOPLIST);						\
		echo "   <key>CFBundleName</key>" >> $(MACINFOPLIST);						\
		echo "   <string>$(PROGRAM)</string>" >> $(MACINFOPLIST);					\
		echo "   <key>CFBundleIdentifier</key>" >> $(MACINFOPLIST);					\
		echo "   <string>com.$(COMPANY).$(PROGRAM)</string>" >> $(MACINFOPLIST);			\
		echo "   <key>CFBundleInfoDictionaryVersion</key>" >> $(MACINFOPLIST);				\
		echo "   <string>6.0</string>" >> $(MACINFOPLIST);						\
		echo "   <key>CFBundlePackageType</key>" >> $(MACINFOPLIST);					\
		echo "   <string>$(BUNDLEPACKAGE)</string>" >> $(MACINFOPLIST);				\
		echo "   <key>CFBundleSignature</key>" >> $(MACINFOPLIST);					\
		echo "   <string>$(BUNDLESIGNATURE)</string>" >> $(MACINFOPLIST);				\
		echo "   <key>CFBundleVersion</key>" >> $(MACINFOPLIST);					\
		echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST);					\
		echo "   <key>CFBundleShortVersionString</key>" >> $(MACINFOPLIST);				\
		echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST);					\
		echo "   <key>CFBundleGetInfoString</key>" >> $(MACINFOPLIST);					\
		echo "   <string>$(PROGRAM), Version $(PROGVER), $(COMPANY)</string>" >> $(MACINFOPLIST);	\
		echo "</dict>" >> $(MACINFOPLIST);								\
		echo "</plist>" >> $(MACINFOPLIST);								\
	fi
#
# install is success ?
#
	@if [ -f $(top_builddir)/$(packagedir)/$(bin_PROGRAMS) ]; then \
		echo "                                               ";\
		echo "===============================================";\
		echo "JaneClone Installation Completed Successfully !";\
		echo "===============================================";\
		echo "                                               ";\
	fi