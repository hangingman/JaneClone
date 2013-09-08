# 依存関係にあるソース群
SUBDIRS = libwxnkf libwxuuid sqlite3 wxsqlite3 curl curlpp js-1.5-rc6a src test

# パッケージ作成用サブディレクトリも設定する
DIST_SUBDIRS = $(SUBDIRS)

# janeclone.wrapper向けルール
bin_SCRIPTS = janeclone
CLEANFILES  = $(bin_SCRIPTS)
EXTRA_DIST  = misc/janeclone.wrapper misc/janeclone.menu test

# 書き換えスクリプト
janecloneexecdir 	= $(datarootdir)/@PACKAGE@
janecloneexecrcdir	= $(datarootdir)/@PACKAGE@/rc

do_subst_wrapper =  sed -e 's,%%%appdir%%%,$(datarootdir)/@PACKAGE@,g' \
			-e 's,%%%pkglibdir%%%,$(pkglibdir),g'
do_subst_menu 	 =  sed -e 's,%%%bindir%%%,$(bindir),g' \
			-e 's,%%%janecloneexecrcdir%%%,$(janecloneexecrcdir),g'

# janecloneスクリプトを作成する
janeclone:
	$(do_subst_wrapper) misc/janeclone.wrapper > janeclone
	chmod +x janeclone
if USE_LINUX
	if [ -d debian ]; then \
		$(do_subst_menu) misc/janeclone.menu > debian/menu; \
	fi
endif

# ChangeLogと連動させる
dist-hook: ChangeLog

# configure.acが更新された時動く
ChangeLog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog

# 依存関係にあるDLL
MINGWBIN= `@WX_CONFIG_PATH@ --prefix`/bin
MINGWLIB= `@WX_CONFIG_PATH@ --prefix`/lib
DEPDLLS = $(shell find $(top_builddir) -type f -name '*.dll')
WXDLLS  = $(shell find `@WX_CONFIG_PATH@ --prefix`/lib -type f -name 'wx*@WX_VERSION_MAJOR@@WX_VERSION_MINOR@*.dll')
STDCDLL	= $(shell find `@WX_CONFIG_PATH@ --prefix` -type f -name 'libstdc++*.dll')
RESTDLL = $(MINGWBIN)/libjpeg-*.dll $(MINGWBIN)/libpng*.dll $(MINGWBIN)/libtiff-*.dll $(MINGWBIN)/libxml2*.dll $(MINGWBIN)/zlib*.dll
LIBGCC  = $(shell find `@WX_CONFIG_PATH@ --prefix` -type f -name 'libgcc*.dll')
JANEEXE = $(top_builddir)/src/.libs/$(PACKAGE_NAME)
EXT	= .exe

# NSISの設定ファイル
MAKENSIS    = makensis
NSIS_NSI_IN = $(top_builddir)/misc/janeclone.nsi.in
NSIS_NSI    = $(top_builddir)/extras/janeclone.nsi

# RPM用設定ファイル
RPM_MACRO   = $(top_builddir)/extras/package/.rpmmacros
# <パッケージ名>-<バージョン番号>.<リリース番号>.spec
SPECFILE	= $(PACKAGE_NAME)-$(VERSION).spec
SPECFILE_PATH	= $(top_builddir)/extras/package/SPECS/$(SPECFILE)
# <パッケージ名>-<バージョン番号>.<リリース番号>.<アーキテクチャ>.rpm
RPMNAME		= $(PACKAGE_NAME)-$(VERSION).`uname -m`.rpm

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

# Windows(x86)向けのインストーラ作成
package-win32-msi: prepare-package
	touch $(NSIS_NSI)
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(LIBGCC) $(top_builddir)/extras/package
	cp -p $(JANEEXE)$(EXT) $(top_builddir)/extras/package
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
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(LIBGCC) $(top_builddir)/extras/package
	cp -p $(JANEEXE)$(EXT) $(top_builddir)/extras/package
	( cd $(top_builddir)/extras && zip -r $(PACKAGE)-win32-$(VERSION).zip package/ && mv $(PACKAGE)-win32-$(VERSION).zip ../ )

# Windows(x64)向けのインストーラ作成
package-win64-msi: prepare-package
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(LIBGCC) $(top_builddir)/extras/package
	cp -p $(JANEEXE)$(EXT) $(top_builddir)/extras/package
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
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(LIBGCC) $(top_builddir)/extras/package
	cp -p $(JANEEXE)$(EXT) $(top_builddir)/extras/package
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
	echo "BuildRequires:	gcc-c++" >> $(SPECFILE_PATH)
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

# Debian系ディストリビューション向けのインストーラ作成
package-deb: remove_distdir dist
# 作業用ディレクトリ作成
	(tar xvf $(distdir).tar.gz && cd $(distdir) && dh_make -y -n -s -e $(PACKAGE_BUGREPORT) -f ../$(distdir).tar.gz)
# controlファイルに詳細を追記
	echo "Source: janeclone" > $(distdir)/debian/control
	echo "Section: net" >> $(distdir)/debian/control
	echo "Priority: extra" >> $(distdir)/debian/control
	echo "Maintainer: hiroyuki nagata <$(PACKAGE_BUGREPORT)>" >> $(distdir)/debian/control
	echo "Build-Depends: debhelper (>= 8.0.0), autotools-dev, libatk1.0-0 (>= 1.29.3), libc6 (>= 2.7), libcairo2 (>= 1.2.4), libdbus-1-3 (>= 1.1.1), libfontconfig1 (>= 2.8.0), libfreetype6 (>= 2.2.1), libgcc1 (>= 1:4.1.1), libglib2.0-0 (>= 2.12.0), libgtk2.0-0 (>= 2.8.0), libpango1.0-0 (>= 1.14.0), libstdc++6 (>= 4.1.1),libwxbase2.8-0 (>= 2.8.10.1), libwxgtk2.8-0 (>= 2.8.10.1), libxml2-dev (>= 2.8.0), fonts-mona" >> $(distdir)/debian/control
	echo "Standards-Version: $(VERSION)" >> $(distdir)/debian/control
	echo "Homepage: https://github.com/Hiroyuki-Nagata/JaneClone" >> $(distdir)/debian/control
	echo "" >> $(distdir)/debian/control
	echo "Package: janeclone" >> $(distdir)/debian/control
	echo "Architecture: any"  >> $(distdir)/debian/control
	echo "Depends: ${shlibs:Depends}, ${misc:Depends}" >> $(distdir)/debian/control
	echo "Description: Cross Platform 2ch browser" >> $(distdir)/debian/control
	echo " JaneClone is a plain browser dedicated to read 2ch thread. Using a library called wxWidgets." >> $(distdir)/debian/control
	echo " It will work on Windows, Linux, Mac. It will keep the platform-specific user interface." >> $(distdir)/debian/control
# rulesファイルに設定を追加
	echo "#!/usr/bin/make -f" > $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
	echo "include /usr/share/cdbs/1/rules/debhelper.mk" >> $(distdir)/debian/rules
	echo "include /usr/share/cdbs/1/class/autotools.mk" >> $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
	echo "" >> $(distdir)/debian/rules
# 必要なファイルをリネームする
	rm -f $(distdir)/debian/menu.ex
# パッケージを作る
	(cd $(distdir) && debuild -uc -us)

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

# OSX向けのバイナリ作成
osx-make-hook : remove_distdir $(BUNDLE) $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME) $(MACICON) $(MACPKGINFO) $(MACINFOPLIST) wxmac-dist-shared-lib

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
MACJANEEXE   = $(BUNDLE)/Contents/MacOS/$(PACKAGE_NAME)

wxmac-dist-shared-lib:
# patch JaneClone subdirectory libraries
	@echo "Copying and Patching JaneClone subdirectory libraries to " $(BINDIR) " ..."
	@for file in `find $(top_builddir) -name \*.dylib -print`; do \
	   echo "Copying " $$file " to " $(BINDIR); \
	   cp -p $$file $(BINDIR); \
	   echo "Patching " $$file "..."; \
	   install_name_tool -id @executable_path/$(BINDIR)/`basename $$file`   \
                             $(BINDIR)/`basename $$file`;                       \
	   install_name_tool -change /usr/local/lib/janeclone/`basename $$file` \
                             @executable_path/$(BINDIR)/`basename $$file`       \
                             $(MACJANEEXE); \
	   for wxlibs in $$(ls $(WXLIBDIR)/*wx*.dylib) ; do                     \
	      install_name_tool -change $$wxlibs                                \
	                        @executable_path/$(BINDIR)/`basename $$wxlibs`  \
	                        $$file ;                                        \
	   done \
	done
# patch all wx libraries
	@echo "Copying wxWidgets dynamic libraries to " $(BINDIR) " ..."
	cp $(WXLIBDIR)/*wx*.dylib $(BINDIR)
	@for file in `ls $(BINDIR)/*wx*.dylib`; do \
	   echo "Patching " $$file "..."; \
	   install_name_tool -id @executable_path/$$file $$file; \
	   install_name_tool -change $(LIBDEFDIR)/`basename $$file` @executable_path/$$file $(MACJANEEXE); \
	done
# end of copying and patching
	@echo "Patching end..."

# OSX向けのインストーラ作成
package-osx-intel32-dmg: prepare-package

# OSX向けのインストーラ作成
package-osx-intel64-dmg: prepare-package

# インストーラ作成のための準備
prepare-package: package-clean
	@echo 'create working directory...'
	mkdir $(top_builddir)/extras
	mkdir $(top_builddir)/extras/package
# check NSIS
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

# 独自設定のクリーン用ターゲット
clean-local: package-clean remove_distdir
# パッケージディレクトリを削除するだけ
package-clean:
# rm packagedir and crate packagedir
	@echo 'delete working directory...'
	rm -rf $(top_builddir)/extras/package/
	rm -rf $(top_builddir)/extras/
	rm -rf $(BUNDLE)
