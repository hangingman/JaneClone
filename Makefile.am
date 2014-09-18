# 依存関係にあるソース群
SUBDIRS = crossguid sqlite3 wxsqlite3 curlpp src test

# パッケージ作成用サブディレクトリも設定する
DIST_SUBDIRS = $(SUBDIRS)

# janeclone.wrapper向けルール
bin_SCRIPTS = janeclone
CLEANFILES  = $(bin_SCRIPTS)
EXTRA_DIST  = misc/janeclone.wrapper misc/janeclone.menu misc/janeclone.desktop test

# 書き換えスクリプト
janecloneexecdir 	= $(datarootdir)/@PACKAGE@
janecloneexecrcdir	= $(datarootdir)/@PACKAGE@/rc

do_subst_wrapper =  sed -e 's,%%%appdir%%%,$(datarootdir)/@PACKAGE@,g' \
			-e 's,%%%pkglibdir%%%,$(pkglibdir),g'
do_subst_menu 	 =  sed -e 's,%%%bindir%%%,$(bindir),g' \
			-e 's,%%%janecloneexecrcdir%%%,$(janecloneexecrcdir),g'

# Doxygen
docs: src/*.cpp src/*.hpp Doxyfile
	doxygen && cd docs/ && git add . && git commit -m 'updated docs' && git push origin gh-pages

# janecloneスクリプトを作成する
janeclone:
	$(do_subst_wrapper) misc/janeclone.wrapper > janeclone
	chmod +x janeclone
if USE_LINUX
	if [ -d debian ]; then \
		$(do_subst_menu) misc/janeclone.menu > debian/menu; \
	fi
endif

# インストール時のファイルコピー(パッケージビルド時は行わない)
install-data-local:
	@if test ! -d debian; then										      \
	   cp -r $(top_builddir)/src/rc $(datarootdir)/@PACKAGE@;						      \
	   if test "x`uname -s`" = "xFreeBSD"; then                   						      \
	      $(do_subst_menu) $(top_builddir)/misc/janeclone.desktop > /usr/local/share/applications/janeclone.desktop; \
	   else                                                                                          		 \
	      $(do_subst_menu) $(top_builddir)/misc/janeclone.desktop > /usr/share/applications/janeclone.desktop;       \
	   fi;														 \
	fi

# ChangeLogと連動させる
dist-hook: ChangeLog

# configure.acが更新された時動く
ChangeLog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog

# NSISの設定ファイル
MAKENSIS    = makensis
NSIS_NSI_IN = $(top_builddir)/misc/janeclone.nsi.in
NSIS_NSI    = $(top_builddir)/extras/janeclone.nsi
NLIST	    = $(shell for x in {1..$(words $(WIN32_DEP))}; do echo $$x; done)
WIN32_DEP   = libstdc++ libgcc libxml2 zlib libcurl libjpeg libpng libtiff ssleay32 libeay32

# RPM用設定ファイル
RPM_MACRO   = $(top_builddir)/extras/package/.rpmmacros
# <パッケージ名>-<バージョン番号>.<リリース番号>.spec
SPECFILE	= $(PACKAGE_NAME)-$(VERSION).spec
SPECFILE_PATH	= $(top_builddir)/extras/package/SPECS/$(SPECFILE)
# <パッケージ名>-<バージョン番号>.<リリース番号>.<アーキテクチャ>.rpm
RPMNAME		= $(PACKAGE_NAME)-$(VERSION).`uname -m`.rpm

# Debian用COPYRIGHTの場所
COPYRIGHT=$(top_builddir)/debian/copyright
PACKAGER="Hiroyuki Nagata"
AUTHOR="Hiroyuki Nagata"

# OSX application bundle用の設定
ICONFILE		= janeclone.icns
ICONPATH		= $(top_builddir)/src/rc/$(ICONFILE)
RCPATH			= $(top_builddir)/src/rc
PROGVER			= $(VERSION)
BUNDLE			= $(top_builddir)/$(PACKAGE_NAME).app
MACICON			= $(BUNDLE)/Contents/$(ICONFILE)
MACPKGINFO		= $(BUNDLE)/Contents/PkgInfo
MACINFOPLIST		= $(BUNDLE)/Contents/Info.plist
# OSX Info.plist用の設定
COMPANY			= ????
BUNDLEPACKAGE		= APPL
BUNDLESIGNATURE		= ???

#
# Windows(x86)向けのインストーラ作成(クロスコンパイラ向け)
#
package-win32-msi: prepare-package check-nsis
	touch $(NSIS_NSI)
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	for f in `find $(top_builddir) -name \*.dll`; do                                                    \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(top_builddir)/src/.libs -name $(PACKAGE_NAME).exe`; do                             \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(bindir) $(libdir) -name wx*@WX_VERSION_MAJOR@@WX_VERSION_MINOR@*.dll`; do          \
	   cp -p $${f} $(top_builddir)/extras/package;         	                                            \
	done
	@$(foreach x,$(WIN32_DEP),                                 \
	   for f in `find $(bindir) $(libdir) -name $(x)*.dll`; do \
	      cp -p $${f} $(top_builddir)/extras/package;          \
	   done;                                                   \
	)
# add line to NSIS script
	cp -p $(NSIS_NSI_IN) $(NSIS_NSI)
	sed -i 's/%%%VERSION%%%/$(PACKAGE_VERSION)/g' $(NSIS_NSI)
	sed -i 's/%%%ARCH%%%/win32/g' $(NSIS_NSI)
	sed -i 's/%%%DEFAULT_PROGRAMFILES%%%/$$PROGRAMFILES32/g' $(NSIS_NSI)
	@echo 'makensis start...'
	$(MAKENSIS) $(NSIS_NSI)

# Windows(x86)向けのzip作成
package-win32-zip: prepare-package
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
	for f in `find $(top_builddir) -name \*.dll`; do                                                    \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(top_builddir)/src/.libs -name $(PACKAGE_NAME).exe`; do                             \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(bindir) $(libdir) -name wx*@WX_VERSION_MAJOR@@WX_VERSION_MINOR@*.dll`; do          \
	   cp -p $${f} $(top_builddir)/extras/package;         	                                            \
	done
	@$(foreach x,$(WIN32_DEP),                                 \
	   for f in `find $(bindir) $(libdir) -name $(x)*.dll`; do \
	      cp -p $${f} $(top_builddir)/extras/package;          \
	   done;                                                   \
	)
	( cd $(top_builddir)/extras && zip -r $(PACKAGE)-win32-$(VERSION).zip package/ && mv $(PACKAGE)-win32-$(VERSION).zip ../ )

#
# Windows(x64)向けのインストーラ作成(クロスコンパイラ向け)
#
package-win64-msi: prepare-package check-nsis
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	for f in `find $(top_builddir) -name \*.dll`; do                                                    \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(top_builddir)/src/.libs -name $(PACKAGE_NAME).exe`; do                             \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(bindir) $(libdir) -name wx*@WX_VERSION_MAJOR@@WX_VERSION_MINOR@*.dll`; do          \
	   cp -p $${f} $(top_builddir)/extras/package;         	                                            \
	done
	@$(foreach x,$(WIN32_DEP),                                 \
	   for f in `find $(bindir) $(libdir) -name $(x)*.dll`; do \
	      cp -p $${f} $(top_builddir)/extras/package;          \
	   done;                                                   \
	)
# add line to NSIS script
	cp -p $(NSIS_NSI_IN) $(NSIS_NSI)
	sed -i 's/%%%VERSION%%%/$(PACKAGE_VERSION)/g' $(NSIS_NSI)
	sed -i 's/%%%ARCH%%%/win64/g' $(NSIS_NSI)
	sed -i 's/%%%DEFAULT_PROGRAMFILES%%%/$$PROGRAMFILES64/g' $(NSIS_NSI)
	@echo 'makensis start...'
	$(MAKENSIS) $(NSIS_NSI)

# Windows(x64)向けのzip作成
package-win64-zip: prepare-package
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
	for f in `find $(top_builddir) -name \*.dll`; do                                                    \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(top_builddir)/src/.libs -name $(PACKAGE_NAME).exe`; do                             \
	   cp -p $${f} $(top_builddir)/extras/package;                                                      \
	done
	for f in `find $(bindir) $(libdir) -name wx*@WX_VERSION_MAJOR@@WX_VERSION_MINOR@*.dll`; do          \
	   cp -p $${f} $(top_builddir)/extras/package;         	                                            \
	done
	@$(foreach x,$(WIN32_DEP),                                 \
	   for f in `find $(bindir) $(libdir) -name $(x)*.dll`; do \
	      cp -p $${f} $(top_builddir)/extras/package;          \
	   done;                                                   \
	)
	( cd $(top_builddir)/extras && zip -r $(PACKAGE)-win64-$(VERSION).zip package/ && mv $(PACKAGE)-win64-$(VERSION).zip ../ )

# RH系ディストリビューション向けのインストーラ作成
package-rpm: package-clean
	@echo 'RPM package create start...'
	mkdir -p $(top_builddir)/extras/package/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
	touch $(RPM_MACRO)
	echo "%_topdir $(top_builddir)/extras/package" > $(RPM_MACRO)
	echo "%packager Hiroyuki Nagata <$(PACKAGE_BUGREPORT)>" >> $(RPM_MACRO)
	echo "%_sysconfdir /etc" >> $(RPM_MACRO)
	$(MAKE) dist
	mv $(distdir).tar.gz $(top_builddir)/extras/package/SOURCES
# SPECファイルを作成する
	touch $(SPECFILE_PATH)
	echo "" > $(SPECFILE_PATH)
	echo "%define		name	$(PACKAGE_NAME)" >> $(SPECFILE_PATH)
	echo "%define		version $(VERSION)" >> $(SPECFILE_PATH)
	echo "%define		release 0" >> $(SPECFILE_PATH)
	echo "%define		prefix  /usr" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "License:		GPL" >> $(SPECFILE_PATH)
	echo "Group:		Applications/Internet" >> $(SPECFILE_PATH)
	echo "Source:		$(distdir).tar.gz" >> $(SPECFILE_PATH)
	echo "Summary:		Cross Platform 2ch browser" >> $(SPECFILE_PATH)
	echo "Packager:		Hiroyuki Nagata" >> $(SPECFILE_PATH)
	echo "Distribution:	`lsb_release -i | awk {'print $$3'}`" >> $(SPECFILE_PATH)
	echo "BuildRoot:	/var/tmp/%{name}-%{version}-root" >> $(SPECFILE_PATH)
	echo "Url:		http://hiroyuki-nagata.github.io/" >> $(SPECFILE_PATH)
	echo "Vendor:		Hiroyuki Nagata<$(PACKAGE_BUGREPORT)>" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "Requires:		filesystem" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "pBuildRequires:	gcc-c++" >> $(SPECFILE_PATH)
	echo "BuildRequires:	ncurses-devel" >> $(SPECFILE_PATH)
	echo "BuildRequires:	zlib-devel" >> $(SPECFILE_PATH)
	echo "BuildRequires:	wxGTK-devel >= 2.8.12" >> $(SPECFILE_PATH)
	echo "BuildRequires:	sqlite-devel" >> $(SPECFILE_PATH)
	echo "BuildRequires:	libxml2-devel" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "%description" >> $(SPECFILE_PATH)
	echo "JaneClone is a plain browser dedicated to read 2ch thread. Using a library called wxWidgets." >> $(SPECFILE_PATH)
	echo "It will work on Windows, Linux, Mac. It will keep the platform-specific user interface." >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "%prep" >> $(SPECFILE_PATH)
	echo "%setup -q" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "%build" >> $(SPECFILE_PATH)
	echo "rm -rf $RPM_BUILD_ROOT" >> $(SPECFILE_PATH)
	echo "CXXFLAGS=$RPM_OPT_FLAGS ./configure --prefix=%{prefix}" >> $(SPECFILE_PATH)
	echo "make" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "%install" >> $(SPECFILE_PATH)
	echo "[ -d $RPM_BUILD_ROOT ] && rm -rf $RPM_BUILD_ROOT;" >> $(SPECFILE_PATH)
	echo "make -e prefix=$RPM_BUILD_ROOT%{prefix} install" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
	echo "%clean" >> $(SPECFILE_PATH)
	echo "[ -d $RPM_BUILD_ROOT ] && rm -rf $RPM_BUILD_ROOT;" >> $(SPECFILE_PATH)
	echo "" >> $(SPECFILE_PATH)
# rpmbuildを実行する
	cd $(top_builddir)/extras/package/SPECS && rpmbuild --macros=$(RPM_MACRO) -ba $(SPECFILE)
	@echo 'RPM package create end...'

#
# Debian系ディストリビューション向けのインストーラ作成
#
if USE_LINUX
   # debian version
   DEBIAN_VERSION=`cat /etc/debian_version`
endif

# target for debian with GPG key
package-deb-with-gpg: package-deb
	@if gpg --version >/dev/null 2>&1; then                             \
	   echo "sign by GPG key...";                                       \
	   (cd $(distdir) && debuild);                                      \
	else                                                                \
	   echo "not use GPG key...if you want to sign, make by terminal."; \
	   (cd $(distdir) && debuild -uc -us);                              \
	fi

# target for debian without GPG key
package-deb-local: package-deb
	echo "not use GPG key...if you want to sign, make by terminal.";
	(cd $(distdir) && debuild -uc -us); 

# update license info
package-deb-license:
	echo "This package was debianized by $(PACKAGER) <$(PACKAGE_BUGREPORT)> on" > $(COPYRIGHT)
	echo `date -R`                                                             >> $(COPYRIGHT)
	echo ""                                                                    >> $(COPYRIGHT)
	echo "It was downloaded from $(UPSTREAM_URL)"                              >> $(COPYRIGHT)
	echo ""                                                                    >> $(COPYRIGHT)
	echo "Upstream Author: $(AUTHOR) <$(PACKAGE_BUGREPORT)>"                   >> $(COPYRIGHT)
	echo ""                                                                    >> $(COPYRIGHT)
	echo "Copyright:  © 2011-" `date +"%Y"` "$(AUTHOR)"                        >> $(COPYRIGHT)
	echo ""                                                                    >> $(COPYRIGHT)
	echo "License:                                         "                   >> $(COPYRIGHT)
	echo ""                                                                    >> $(COPYRIGHT)
	cat $(top_builddir)/COPYING |                                              >> $(COPYRIGHT)
	echo ""
	echo "-----------------------------------------------------------------------------" >> $(COPYRIGHT)
	echo ""
#sh $(top_builddir)/misc/license.sh >> $(COPYRIGHT)

# target for debian
package-deb: package-deb-license remove_distdir dist
	(tar xvf $(distdir).tar.gz && cd $(distdir) && dh_make -y -n -s -e $(PACKAGE_BUGREPORT) -f ../$(distdir).tar.gz)
	@if test "$(DEBIAN_VERSION)" = "*wheezy*"; then                        \
	   echo "use Debian wheezy control file...";                           \
	   cp $(top_builddir)/debian/control.wheezy $(distdir)/debian/control; \
	else                                                                   \
	   echo "use Debian jessie control file...";                           \
	   cp $(top_builddir)/debian/control.jessie $(distdir)/debian/control; \
	fi
	sed -i 's/%%%VERSION%%%/$(PACKAGE_VERSION)/g' $(distdir)/debian/control             #  upstream version 
	sed -i 's/%%%PACKAGE_BUGREPORT%%%/$(PACKAGE_BUGREPORT)/g' $(distdir)/debian/control #  bugreport address
	cp $(top_builddir)/debian/rules   $(distdir)/debian/rules                           #  rule file
	rm -f $(distdir)/debian/*.ex                                                        #  delete extension files

remove_distdir :
# パッケージを削除
	if test -d "$(distdir)"; then \
	find "$(distdir)" -type d ! -perm -200 -exec chmod u+w {} ';' \
	&& rm -rf "$(distdir)" \
	|| { sleep 5 && rm -rf "$(distdir)"; }; \
	else :; fi
	rm -rf $(distdir).tar.gz
	rm -rf $(distdir).zip
	rm -rf janeclone_*
	rm -rf $(BUNDLE)

# OSX向けのバイナリ(テスト用)
osx-make-bin : remove_distdir $(BUNDLE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME) $(MACICON) $(MACPKGINFO) $(MACINFOPLIST) wxmac-dist-shared-lib

# OSX向けのバイナリ作成
osx-make-hook : remove_distdir $(BUNDLE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME) $(MACICON) $(MACPKGINFO) $(MACINFOPLIST) check-dylibbundler wxmac-dist-shared-lib package-osx-intel64-dmg

# Bundleその他の生成方法を書く
#  This builds the bundle's directory structure.
$(BUNDLE) :
	@echo "==== Building bundle directory structure ===="
	mkdir -p $(BUNDLE)/Contents
	mkdir -p $(BUNDLE)/Contents/MacOS
	mkdir -p $(BUNDLE)/Contents/Resources

#  This builds the executable right inside the bundle.
$(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME):
	@echo "==== Copying executable ===="
	cp -f $(JANEEXE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME)
	cp -rf $(RCPATH) $(BUNDLE)/Contents/MacOS

#  This copies the icon file into the bundle.
$(MACICON):
	@echo "==== Copying icon file into bundle ===="
	cp -f $(ICONPATH) $(BUNDLE)/Contents/Resources/$(ICONFILE)

#  This creates the Contents/PkgInfo file.
$(MACPKGINFO):
	@echo "==== Creating PkgInfo ===="
	touch $(MACPKGINFO)
	@echo -n "$(BUNDLEPACKAGE)$(BUNDLESIGNATURE)" > $(MACPKGINFO)

#  This creates the Contents/Info.plist file.
$(MACINFOPLIST):
	@echo "==== Creating Info.plist ===="
	touch $(MACINFOPLIST)
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" >> $(MACINFOPLIST)
	@echo "<!DOCTYPE plist PUBLIC " >> $(MACINFOPLIST)
	@echo "\"-//Apple Computer//DTD PLIST 1.0//EN\" " >> $(MACINFOPLIST)
	@echo "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $(MACINFOPLIST)
	@echo "<plist version=\"1.0\">" >> $(MACINFOPLIST)
	@echo "<dict>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleDevelopmentRegion</key>" >> $(MACINFOPLIST)
	@echo "   <string>Japanese</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleExecutable</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PACKAGE_NAME)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleIconFile</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(ICONFILE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleName</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PACKAGE_NAME)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleIdentifier</key>" >> $(MACINFOPLIST)
	@echo "   <string>com.$(COMPANY).$(PACKAGE_NAME)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleInfoDictionaryVersion</key>" >> $(MACINFOPLIST)
	@echo "   <string>6.0</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundlePackageType</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(BUNDLEPACKAGE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleSignature</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(BUNDLESIGNATURE)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleVersion</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleShortVersionString</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PROGVER)</string>" >> $(MACINFOPLIST)
	@echo "   <key>CFBundleGetInfoString</key>" >> $(MACINFOPLIST)
	@echo "   <string>$(PACKAGE_NAME), Version $(PROGVER), $(COMPANY)</string>" >> $(MACINFOPLIST)
	@echo "</dict>" >> $(MACINFOPLIST)
	@echo "</plist>" >> $(MACINFOPLIST)

# OSX向けにライブラリをBUNDLEに設定する
WXLIBPOSTFIX = *wx*@WX_VERSION_MAJOR@.@WX_VERSION_MINOR@.@WX_VERSION_MICRO@.dylib
WXLIBDIR     = `@WX_CONFIG_PATH@ --prefix`/lib
BINDIR	     = $(BUNDLE)/Contents/MacOS
LIBDEFDIR    = /usr/local/lib
JANEEXE      = $(top_builddir)/src/.libs/$(PACKAGE_NAME)
MACJANEEXE   = $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME)
DYLIBBUNDLER = dylibbundler

wxmac-dist-shared-lib:
	@echo "Copying and Patching JaneClone subdirectory libraries to " $(BINDIR)/lib " ..."
	mkdir $(BINDIR)/lib
	@for dylibs in `otool -L $(MACJANEEXE) | grep -o /.\*.dylib | sort -u `; do \
	   if [[ $$dylibs != /usr/lib/* ]]; then                                    \
	      echo "Copying " $$dylibs " to " $(BINDIR)/lib;                        \
	      cp -p $$dylibs $(BINDIR)/lib;                                         \
	      echo "Patching " $$dylibs " to " $(BINDIR)/lib;                       \
	      install_name_tool -change $$dylibs                                    \
	                        @executable_path/lib/`basename $$dylibs`            \
	                        $(MACJANEEXE);                                      \
	   fi                                                                       \
	done
	@echo "=====================================================================";
	@echo "End of JaneClone's dependency patching...                            ";
	@echo "=====================================================================";
	@for deplibs in `otool -L $(BINDIR)/lib/*.dylib | grep -o /.\*dylib | sort -u`; do   \
	   if [[ $$deplibs = /usr/local/lib/* ]] || [[ $$deplibs = /opt/local/lib/* ]]; then \
	      if [[ ! -e $(BINDIR)/lib/`basename $$deplibs` ]]; then \
	         echo "Copying " $$deplibs " to " $(BINDIR)/lib;     \
	         cp -p $$deplibs $(BINDIR)/lib;                      \
	      fi;                 \
	   fi;                    \
	done
	@echo "=====================================================================";
	@echo "End of JaneClone's dependency libraries copying...                   ";
	@echo "=====================================================================";
	@for file in `ls $(BINDIR)/lib/*.dylib` ; do \
	   otool -L $$file | egrep -v /\(usr/lib\|System\) | awk {'print $$1'} | while read line; \
	   do                 \
	      if [[ $$line != *: ]]; then \
	         echo "Patching " $$line " to @executable_path/lib/" `basename $$line` ;              \
	         install_name_tool -change $${line} @executable_path/lib/`basename $$line` $${file};  \
	      fi                          \
	   done                           \
	done

# OSX向けのインストーラ作成
package-osx-intel64-dmg:
	@echo "!!! Start make disk image file !!!";
	hdiutil create -ov -srcfolder $(BUNDLE) -fs HFS+ -format UDZO \
               -imagekey zlib-level=9 -volname "JaneClone.app" $(PACKAGE)-osx64-$(VERSION).dmg;
	@echo "!!! Finish make disk image file !!!";

# インストーラ作成のための準備
prepare-package: package-clean
	@echo 'create working directory...'
	mkdir $(top_builddir)/extras
	mkdir $(top_builddir)/extras/package
# check NSIS
check-nsis:
	@echo 'check NSIS...'
	@if makensis -VERSION >/dev/null 2>&1; then \
	    MAKENSIS="makensis"; \
	elif [ -x "/cygdrive/c/Program Files/NSIS/makensis" ]; then \
	    MAKENSIS="/cygdrive/c/Program\ Files/NSIS/makensis"; \
	elif [ -x "$(PROGRAMFILES)/NSIS/makensis" ]; then \
	    MAKENSIS="$(PROGRAMFILES)/NSIS/makensis"; \
	elif wine --version >/dev/null 2>&1; then \
	    MAKENSIS="wine C:/Program\ Files/NSIS/makensis.exe"; \
	else \
	    echo 'Error: cannot locate makensis tool'; exit 1; \
	fi; \
	echo 'NSIS is exist'

# check dylibbundler
check-dylibbundler:
	@echo 'check dylibbundler...'
	@if dylibbundler -VERSION >/dev/null 2>&1; then \
	    DYLIBBUNDLER="dylibbundler"; \
	elif [ -x "/usr/local/bin/dylibbundler" ]; then \
	    DYLIBBUNDLER="/usr/local/bin/dylibbundler"; \
	else \
	    echo 'Error: cannot locate dylibbundler'; exit 1; \
	fi; \
	echo 'dylibbundler is exist'


# 独自設定のクリーン用ターゲット
clean-local: package-clean remove_distdir
# パッケージディレクトリを削除するだけ
package-clean:
# rm packagedir and crate packagedir
	@echo 'delete working directory...'
	rm -rf $(top_builddir)/extras/package/
	rm -rf $(top_builddir)/extras/
	rm -rf $(BUNDLE)
