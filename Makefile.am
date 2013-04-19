
if USE_WINDRES
   # 依存関係にあるソース群
   SUBDIRS = libwxnkf libwxuuid sqlite3 wxsqlite3 src
else
   # 依存関係にあるソース群
   SUBDIRS = libwxnkf libwxuuid wxsqlite3 src
endif

# パッケージ作成用サブディレクトリも設定する
DIST_SUBDIRS = $(SUBDIRS)

# ChangeLogと連動させる
dist-hook: ChangeLog

# configure.acが更新された時動く
ChangeLog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog

# 依存関係にあるDLL
MINGWBIN= /mingw/bin
DEPDLLS = $(shell find $(top_builddir) -type f -name '*.dll')
WXDLLS  = $(shell find `wx-config --prefix`/lib -type f -name 'wx*.dll')
STDCDLL	= $(shell find /mingw/bin -type f -name 'libstdc++*.dll')
RESTDLL = $(MINGWBIN)/libjpeg-*.dll $(MINGWBIN)/libpng*.dll $(MINGWBIN)/libtiff-*.dll $(MINGWBIN)/libxml2*.dll $(MINGWBIN)/zlib*.dll
LIBGCC64= $(MINGWBIN)/libgcc_s_sjlj_64*.dll
LIBGCC	= $(MINGWBIN)/libgcc_s_sjlj*.dll
JANEEXE = $(top_builddir)/src/.libs/$(PACKAGE_NAME)

# NSISの設定ファイル
MAKENSIS    = makensis
NSIS_NSI_IN = $(top_builddir)/janeclone.nsi.in
NSIS_NSI    = $(top_builddir)/extras/janeclone.nsi

# RPM用設定ファイル
RPM_MACRO   = $(top_builddir)/extras/package/.rpmmacros
# <パッケージ名>-<バージョン番号>.<リリース番号>.spec
SPECFILE	= $(PACKAGE_NAME)-$(VERSION).spec
SPECFILE_PATH	= $(top_builddir)/extras/package/SPECS/$(SPECFILE)
# <パッケージ名>-<バージョン番号>.<リリース番号>.<アーキテクチャ>.rpm
RPMNAME		= $(PACKAGE_NAME)-$(VERSION).`uname -m`.rpm

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
	cp -p $(JANEEXE) $(top_builddir)/extras/package
# add line to NSIS script
	cp -p $(NSIS_NSI_IN) $(NSIS_NSI)
	sed -i 's/%%%VERSION%%%/$(PACKAGE_VERSION)/g' $(NSIS_NSI)
	sed -i 's/%%%ARCH%%%/win32/g' $(NSIS_NSI)
	@echo 'makensis start...'
	$(MAKENSIS) $(NSIS_NSI)

# Windows(x64)向けのインストーラ作成
package-win64-msi: prepare-package
	touch $(NSIS_NSI)
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/src/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(LIBGCC64) $(top_builddir)/extras/package
	cp -p $(JANEEXE) $(top_builddir)/extras/package
# add line to NSIS script
	cp -p $(NSIS_NSI_IN) $(NSIS_NSI)
	sed -i 's/%%%VERSION%%%/$(PACKAGE_VERSION)/g' $(NSIS_NSI)
	sed -i 's/%%%ARCH%%%/win64/g' $(NSIS_NSI)
	@echo 'makensis start...'
	$(MAKENSIS) $(NSIS_NSI)

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
package-deb: prepare-package

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
clean-local: package-clean
# パッケージディレクトリを削除するだけ
package-clean:
# rm packagedir and crate packagedir
	@echo 'delete working directory...'
	rm -rf $(top_builddir)/extras/package/
	rm -rf $(top_builddir)/extras/
