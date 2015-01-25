#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([JaneClone], [1.1.5], [newserver002@gmail.com])

# SCM URL
AC_SUBST(UPSTREAM_URL, [https://github.com/Hiroyuki-Nagata/JaneClone])
# Refering Debian Policy Version 
# -> http://www.debian.or.jp/community/devel/debian-policy-ja/policy.ja.html/
AC_SUBST(DEB_STANDARDS_VERSION, [3.9.3])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wno-portability])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/enums.hpp])
AC_CONFIG_HEADERS([config.h:curlpp/include/curlpp/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_SYS_LARGEFILE
AS_IF([test "$prefix" = "NONE"], 
      [AC_DEFINE_UNQUOTED([PREFIX_DIR], ["/usr/local"], [--prefix dir macro])],
      [AC_DEFINE_UNQUOTED([PREFIX_DIR], ["$prefix"],    [--prefix dir macro])]
)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
PKG_PROG_PKG_CONFIG
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Set boolean value for Makefile.am
AM_CONDITIONAL(USE_MAC_OSX, false)
AM_CONDITIONAL(USE_LINUX, false)
AM_CONDITIONAL(USE_WX_WEBVIEW, false)
AM_CONDITIONAL(BUILDING_SQLITE3, false)
AM_CONDITIONAL(BUILDING_WXSQLITE3, false)
AM_CONDITIONAL(BUILDING_CURLPP, false)

# Checks for HostOS
case "$host" in
        *-*-mingw*|*-*-cygwin*)
                PLATFORM_STRING="Win32"
		AC_SUBST(PLATFORM_STRING)
		need_no_undefined=yes
                ;;
        *-*-linux*)
                PLATFORM_STRING="Linux"
		AC_SUBST(PLATFORM_STRING)
		AM_CONDITIONAL(USE_LINUX, true)
		need_no_undefined=no
                ;;
        *-*-darwin*)
                PLATFORM_STRING="Apple"
		AC_SUBST(PLATFORM_STRING)
		AM_CONDITIONAL(USE_MAC_OSX, true)
		need_no_undefined=no
                ;;
        *-*-*bsd*)
                PLATFORM_STRING="BSD"
		AC_SUBST(PLATFORM_STRING)
		need_no_undefined=no
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac

# Checks for libtool "no undefined"
AM_CONDITIONAL(NO_UNDEFINED, test x$need_no_undefined = xyes)
if test x$need_no_undefined = xyes; then
   AC_MSG_RESULT([define -no-undefined ...])
fi

# Check for windres
if test "$PLATFORM_STRING" = "Win32"; then
   AC_CHECK_TOOL([WINDRES], [windres], [no])
   if test "x$WINDRES" = "xno"; then
      AC_MSG_ERROR([Cannot find windres])
   fi
   AC_SUBST(WINDRES)
   AM_CONDITIONAL(USE_WINDRES, true)
else
   # Set false use_windres
   AM_CONDITIONAL(USE_WINDRES, false)
fi

# Checks for libxml2
AC_PATH_PROGS(XML2CONFIG, xml2-config)
AC_MSG_CHECKING(for special libxml2 includes) 
if test "x$XML2CONFIG" = "x"; then 
   AC_MSG_RESULT(libxml2 config not found) 
   else 
   XML2CFLAGS=`$XML2CONFIG --cflags` 
   AC_MSG_RESULT($XML2CFLAGS)
   AC_SUBST(XML2CFLAGS)
   AC_CHECK_LIB(xml2, xmlReadMemory)
   XML2LIBS=`$XML2CONFIG --libs`
   AC_MSG_RESULT($XML2LIBS)
   AC_SUBST(XML2LIBS)
fi

#-----------------------------------------------------------------------
# Checks for wxWebView feature
#
AC_ARG_ENABLE([webview],
    AS_HELP_STRING([--enable-webview], [Enable feature to connect with each platform browser]))

AS_IF([test "x$enable_webview" = "xyes"], [
   # Check wxWidgets version
   WX_CONFIG_CHECK([2.9.3], [has_webview=1], [has_webview=0], [webview])
   AC_MSG_CHECKING([for whether to support wxwebview])
   if test $has_webview = 1; then
      AM_CONDITIONAL(USE_WX_WEBVIEW, true)
      AC_DEFINE(USE_WX_WEBVIEW, 1, [when building wxwebview code])
      AC_MSG_RESULT(yes)
   else
      AC_MSG_RESULT(no)
   fi
])

#-----------------------------------------------------------------------
# Checks for wxWidgets
#
AM_OPTIONS_WXCONFIG
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

# Add WX Flags
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

# Checks for zlib
AC_CHECK_LIB(z,gzclose,LIBS="-lz $LIBS" has_zlib=1,has_zlib=0)
if test $has_zlib = 0; then
  AC_MSG_ERROR("ERROR: cannot find zlib.")
fi
# Checks for pthread
AC_CHECK_LIB(pthread,pthread_create,has_pthread=1,has_pthread=0)
if test $has_pthread = 0; then
  AC_MSG_ERROR("ERROR: cannot find pthread.")
fi
# Checks for libsqlite3 ( if not exist, build it )
AC_CHECK_LIB(sqlite3,sqlite3_libversion_number,has_sqlite3=1,has_sqlite3=0)
if test $has_sqlite3 = 0; then
  AC_MSG_NOTICE([we will build sqlite3...])
  AM_CONDITIONAL(BUILDING_SQLITE3, true)
fi
# Checks for libwxsqlite3 ( if not exist, build it )
AC_MSG_CHECKING([for wxsqlite3])
AC_LANG_PUSH(C++)
SAVE_LIBS="$LIBS"
LIBS="$LIBS -lwxsqlite3-$wx_config_major_version"".""$wx_config_minor_version"
AC_TRY_LINK([#include <wx/wxsqlite3.h>], 
	    [wxSQLite3Database db;],
	    has_wxsqlite3=1,
	    has_wxsqlite3=0)
AC_LANG_POP(C++)
if test $has_wxsqlite3 = 0; then
  AC_MSG_RESULT([no])
  AC_MSG_NOTICE([we will build wxsqlite3...])
  AM_CONDITIONAL(BUILDING_WXSQLITE3, true)
  LIBS="$SAVE_LIBS"
else
  AC_MSG_RESULT([yes])
fi

#-----------------------------------------------------------------------
#   libcurl build flag
#

dnl -------
dnl set my_cv_curl_vers to the version of libcurl or NONE
dnl if libcurl is not found or is too old
AC_DEFUN([MY_CURL],[
 AC_CACHE_VAL(my_cv_curl_vers,[
 my_cv_curl_vers=NONE
 dnl check is the plain-text version of the required version
 check="7.10.0"
 dnl check_hex must be UPPERCASE if any hex letters are present
 check_hex="070A00"
 dnl for cross compiling, variable curl-config 
 AC_PATH_PROGS(CURLCONFIG, curl-config)
 
 AC_MSG_CHECKING([for curl >= $check])
 
 if eval $CURLCONFIG --version 2>/dev/null >/dev/null; then
   ver=`$CURLCONFIG --version | sed -e "s/libcurl //g"`
   hex_ver=`$CURLCONFIG --vernum | tr 'a-f' 'A-F'`
   ok=`echo "ibase=16; if($hex_ver>=$check_hex) $hex_ver else 0" | bc`
 
   if test x$ok != x0; then
     my_cv_curl_vers="$ver"
     AC_MSG_RESULT([$my_cv_curl_vers])
     CURL_LIBS=`$CURLCONFIG --libs`
     CURL_CFLAGS=`$CURLCONFIG --cflags`
     LIBS="$LIBS $CURL_LIBS"
   else
     AC_MSG_RESULT(FAILED)
     AC_MSG_WARN([$ver is too old. Need version $check or higher.])
   fi
 else
   AC_MSG_RESULT(FAILED)
   AC_MSG_WARN([curl-config was not found])
 fi
 ])
])

MY_CURL
AC_SUBST(CURL_CFLAGS)

# Checks for curlpp ( if not exist, build it )
AC_MSG_CHECKING([for curlpp])
AC_LANG_PUSH(C++)
SAVE_LIBS="$LIBS"
LIBS="$LIBS -lcurlpp"
AC_TRY_LINK([#include <curlpp/Easy.hpp>], 
	    [curlpp::Easy easy;],
	    has_curlpp=1,
	    has_curlpp=0)
AC_LANG_POP(C++)
if test $has_curlpp = 0; then
  AC_MSG_RESULT([no])
  AC_MSG_NOTICE([we will build curlpp...])
  AM_CONDITIONAL(BUILDING_CURLPP, true)
  LIBS="$SAVE_LIBS"
  AC_DEFINE(BUILDING_CURLPP, 1, [when building cURLpp itself])
  AC_MSG_CHECKING([if we need CURLPP_STATICLIB])
  if test "X$enable_shared" = "Xno"
  then
    AC_DEFINE(CURLPP_STATICLIB, 1, [when not building a shared library])
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
  fi
else
  AC_MSG_RESULT([yes])
fi
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# janeclone shingetsu support, default is disable
#
AC_ARG_ENABLE([shingetsu],
    AS_HELP_STRING([--enable-shingetsu], [Enable feature to connect p2p board system shingetsu]))

AS_IF([test "x$enable_bar" = "xyes"], [
    AC_DEFINE(USE_SHINGETSU, 1, [when building shingetsu support code])
])
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   libcrypto for md5 hash
#
AS_IF([test x"$PLATFORM_STRING" != x"Apple"], [
    PKG_CHECK_MODULES([CRYPTO], [libcrypto],, [
        AC_CHECK_LIB([crypto], [MD5], [CRYPTO_LIBS="-lcrypto"], [
            AC_MSG_ERROR("ERROR: cannot find libcrypto.")
        ])
    ])
])
#-----------------------------------------------------------------------

# Check compile flag
AX_CHECK_COMPILE_FLAG([-fstack-protector], [CXXFLAGS="$CXXFLAGS -fstack-protector"])
AX_CHECK_COMPILE_FLAG([-fomit-frame-pointer], [CXXFLAGS="$CXXFLAGS -fomit-frame-pointer"])
AX_CHECK_COMPILE_FLAG([-Wno-invalid-source-encoding],[CXXFLAGS="$CXXFLAGS -Wno-invalid-source-encoding"])
AX_CHECK_COMPILE_FLAG([-Wno-parentheses],[CXXFLAGS="$CXXFLAGS -Wno-parentheses"])

# Checks for compiler
AX_COMPILER_VENDOR
case "${ax_cv_c_compiler_vendor}" in

   gnu)
      	AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
	;;

   clang)
	if test "$PLATFORM_STRING" = "Apple"; then
	   CXXFLAGS="$CXXFLAGS -stdlib=libc++"
	fi
	   AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
        ;;	
   *)
	AC_MSG_ERROR("ERROR: JaneClone cannot compile with this compiler.")
	;;
esac

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h langinfo.h locale.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset munmap fdatasync usleep fullfsync localtime_r gmtime_r])
AC_FUNC_STRERROR_R

# Setting for generate Makefile location
AC_CONFIG_FILES([Makefile
		 cmdline_twitter/Makefile
                 src/Makefile
		 test/Makefile])
# sqlite3
AM_COND_IF([BUILDING_SQLITE3],	
	   [AC_CONFIG_FILES([sqlite3/Makefile]) ])
# wxsqlite3
AM_COND_IF([BUILDING_WXSQLITE3],
	   [AC_CONFIG_FILES([wxsqlite3/Makefile]) ])
# curlpp
AM_COND_IF([BUILDING_CURLPP],
	   [AC_CONFIG_FILES([curlpp/Makefile			      
			     curlpp/src/Makefile		      
			     curlpp/src/curlpp/Makefile		      
			     curlpp/src/curlpp/internal/Makefile      
			     curlpp/src/utilspp/Makefile	      
			     curlpp/include/Makefile		      
			     curlpp/include/curlpp/Makefile	      
			     curlpp/include/curlpp/internal/Makefile  
			     curlpp/include/utilspp/Makefile	      
			     curlpp/include/utilspp/functor/Makefile  
			     curlpp/include/utilspp/singleton/Makefile])])

# Output
AC_OUTPUT

# Trace all options
AC_MSG_RESULT([])
AC_MSG_RESULT([Target:	  $PLATFORM_STRING])
AC_MSG_RESULT([CC:	  $CC])
AC_MSG_RESULT([CXX:	  $CXX])
AC_MSG_RESULT([CPPFLAGS:  $CPPFLAGS])
AC_MSG_RESULT([CXXFLAGS:  $CXXFLAGS])
AC_MSG_RESULT([LIBS:	  $LIBS])
AM_COND_IF([BUILDING_SQLITE3],	
	   [AC_MSG_RESULT([sqlite3  :	  builtin])],
	   [AC_MSG_RESULT([sqlite3  :	  sys])])
AM_COND_IF([BUILDING_WXSQLITE3],
	   [AC_MSG_RESULT([wxsqlite3:	  builtin])],
	   [AC_MSG_RESULT([wxsqlite3:	  sys])])
AM_COND_IF([BUILDING_CURLPP],
	   [AC_MSG_RESULT([curlpp   :	  builtin])],
	   [AC_MSG_RESULT([curlpp   :	  sys])])
