SUBDIRS = libwxnkf libwxuuid sqlite3 wxsqlite3 src

# 依存関係にあるDLL
MINGWBIN= /mingw/bin
DEPDLLS = $(shell find $(top_builddir) -type f -name '*.dll')
WXDLLS  = $(shell find `wx-config --prefix`/lib -type f -name 'wx*.dll')
STDCDLL	= $(shell find /mingw/bin -type f -name 'libstdc++*.dll')
RESTDLL = $(MINGWBIN)/libjpeg-*.dll $(MINGWBIN)/libpng*.dll $(MINGWBIN)/libtiff-*.dll $(MINGWBIN)/libxml2*.dll $(MINGWBIN)/zlib*.dll 
JANEEXE = $(top_builddir)/src/.libs/$(PACKAGE_NAME)

# NSISの設定ファイル
NSIS_NSI_IN = $(top_builddir)/janeclone.nsi.in
NSIS_NSI    = $(top_builddir)/extras/janeclone.nsi

# Windows(x86)向けのインストーラ作成
package-win32-msi: prepare-package
	touch $(NSIS_NSI)
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(JANEEXE) $(top_builddir)/extras/package
# add line to NSIS script
	cp -p $(NSIS_NSI_IN) $(NSIS_NSI)
	sed -i 's/%%%VERSION%%%/$(PACKAGE_VERSION)/g' $(NSIS_NSI)
	@echo 'makensis start...'
	$(MAKENSIS) $(NSIS_NSI)

# Windows(x64)向けのインストーラ作成
package-win64-msi: prepare-package
	touch $(NSIS_NSI)
	cp -p $(top_builddir)/INSTALL $(top_builddir)/extras/package
	cp -p $(top_builddir)/README $(top_builddir)/extras/package
	cp -rp $(top_builddir)/rc $(top_builddir)/extras/package
# find dlls and copy to package directory
	cp -p $(DEPDLLS) $(top_builddir)/extras/package
	cp -p $(WXDLLS) $(top_builddir)/extras/package
	cp -p $(STDCDLL) $(top_builddir)/extras/package
	cp -p $(RESTDLL) $(top_builddir)/extras/package
	cp -p $(JANEEXE) $(top_builddir)/extras/package

# RH系ディストリビューション向けのインストーラ作成
package-rpm: prepare-package

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
# パッケージディレクトリを削除するだけ
package-clean:
# rm packagedir and crate packagedir
	@echo 'delete working directory...'
	rm -rf $(top_builddir)/extras/package/
	rm -rf $(top_builddir)/extras/