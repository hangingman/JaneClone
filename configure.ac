#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([JaneClone], [1.1.2], [newserver002@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wno-portability])
AC_CONFIG_SRCDIR([libwxnkf/flagset.h])
AC_CONFIG_HEADERS([config.h:curlpp/include/curlpp/config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_SYS_LARGEFILE

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for HostOS
case "$host" in
        *-*-mingw*|*-*-cygwin*)
                AC_DEFINE(PLATFORM_WIN32, 1, [Platform is Win32])
                PLATFORM_STRING="Win32"
		AC_SUBST(PLATFORM_STRING)
		need_no_undefined=no
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                PLATFORM_STRING="Linux"
		AC_SUBST(PLATFORM_STRING)
		need_no_undefined=no
                ;;
        *-*-darwin*)
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                PLATFORM_STRING="Apple"
		AC_SUBST(PLATFORM_STRING)
		need_no_undefined=no
                ;;
        *-*-*bsd*)
                AC_DEFINE(PLATFORM_BSD, 1, [Platform is BSD])
                PLATFORM_STRING="BSD"
		AC_SUBST(PLATFORM_STRING)
		need_no_undefined=no
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac

# Checks for no undefined
AM_CONDITIONAL(NO_UNDEFINED, test x$need_no_undefined = xyes)
AM_CONDITIONAL(USE_MAC_OSX, false)
AM_CONDITIONAL(USE_LINUX, false)

# Check for windres
if test "$PLATFORM_STRING" = "Win32"; then
   AC_CHECK_TARGET_TOOL([WINDRES], [windres], [no])
   if test "x$WINDRES" = "xno"; then
      AC_MSG_ERROR([Cannot find windres])
   fi
   AC_SUBST(WINDRES)
   AM_CONDITIONAL(USE_WINDRES, true)
else
# Set false use_windres
AM_CONDITIONAL(USE_WINDRES, false)
# Set MacOSX flag
  if test "$PLATFORM_STRING" = "Apple"; then
     AM_CONDITIONAL(USE_MAC_OSX, true)
  fi
# Set Linux flag
  if test "$PLATFORM_STRING" = "Linux"; then
     AM_CONDITIONAL(USE_LINUX, true)
  fi
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

# Checks for wxWidgets.
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

#-----------------------------------------------------------------------
#   --enable-readline
#
AC_ARG_ENABLE(readline, [AS_HELP_STRING(
  [--enable-readline], 
  [use readline in shell tool (yes, no) [default=yes]])], 
  [], [enable_readline=yes])
if test x"$enable_readline" != xno ; then
  sLIBS=$LIBS
  LIBS=""
  AC_SEARCH_LIBS(tgetent, curses ncurses ncursesw, [], [])
  AC_SEARCH_LIBS(readline, readline, [], [enable_readline=no])
  AC_CHECK_FUNCS(readline, [], [])
  READLINE_LIBS=$LIBS
  LIBS=$sLIBS
fi
AC_SUBST(READLINE_LIBS)
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --enable-threadsafe
#
AC_ARG_ENABLE(threadsafe, [AS_HELP_STRING(
  [--enable-threadsafe], [build a thread-safe library [default=yes]])], 
  [], [enable_threadsafe=yes])
THREADSAFE_FLAGS=-DSQLITE_THREADSAFE=0
if test x"$enable_threadsafe" != "xno"; then
  THREADSAFE_FLAGS="-D_REENTRANT=1 -DSQLITE_THREADSAFE=1"
  AC_SEARCH_LIBS(pthread_create, pthread)
fi
AC_SUBST(THREADSAFE_FLAGS)
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
#   --enable-dynamic-extensions
#
AC_ARG_ENABLE(dynamic-extensions, [AS_HELP_STRING(
  [--enable-dynamic-extensions], [support loadable extensions [default=yes]])], 
  [], [enable_dynamic_extensions=yes])
if test x"$enable_dynamic_extensions" != "xno"; then
  AC_SEARCH_LIBS(dlopen, dl)
else
  DYNAMIC_EXTENSION_FLAGS=-DSQLITE_OMIT_LOAD_EXTENSION=1
fi
AC_MSG_CHECKING([for whether to support dynamic extensions])
AC_MSG_RESULT($enable_dynamic_extensions)
AC_SUBST(DYNAMIC_EXTENSION_FLAGS)
#-----------------------------------------------------------------------

# Checks for zlib
AC_CHECK_LIB(z,gzopen,has_zlib=1,has_zlib=0)
if test $has_zlib = 0; then
  AC_MSG_ERROR("ERROR: cannot find zlib.")
fi
# Checks for pthread
AC_CHECK_LIB(pthread,pthread_create,has_pthread=1,has_pthread=0)
if test $has_pthread = 0; then
  AC_MSG_ERROR("ERROR: cannot find pthread.")
fi

# curlpp build flag
AC_MSG_CHECKING([if we need BUILDING_CURLPP])
case $host in
  *-*-mingw*)
    AC_DEFINE(BUILDING_CURLPP, 1, [when building cURLpp itself])
    AC_MSG_RESULT(yes)
    AC_MSG_CHECKING([if we need CURLPP_STATICLIB])
    if test "X$enable_shared" = "Xno"
    then
      AC_DEFINE(CURLPP_STATICLIB, 1, [when not building a shared library])
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
    fi
    ;;
  *)
    AC_MSG_RESULT(no)
    ;;
esac

# Check compile flag
AX_CHECK_COMPILE_FLAG([-fno-stack-protector], [CXXFLAGS="$CXXFLAGS -fno-stack-protector"])
AX_CHECK_COMPILE_FLAG([-fomit-frame-pointer], [CXXFLAGS="$CXXFLAGS -fomit-frame-pointer"])

# Checks for compiler
AX_COMPILER_VENDOR
case "${ax_cv_c_compiler_vendor}" in

   gnu)
	if test "$PLATFORM_STRING" = "Win32"; then
   	   AX_CHECK_COMPILE_FLAG([-std=gnu++0x], [CXXFLAGS="$CXXFLAGS -std=gnu++0x"])
	else
	   AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -std=c++0x"])
	fi
	;;
   clang)
	if test "$PLATFORM_STRING" = "BSD"; then
	   CXXFLAGS="$CXXFLAGS -std=c++11"
	else
	   CXXFLAGS="$CXXFLAGS -std=c++11 -stdlib=libc++"
	fi
        ;;	
   *)
	AC_MSG_ERROR("ERROR: JaneClone cannot compile with gnu gcc or clang.")
	;;
esac

# Add Flags
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

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
AC_CHECK_FUNCS([memset munmap nl_langinfo strcasecmp strchr strcspn strpbrk strrchr strspn strstr fdatasync usleep fullfsync localtime_r gmtime_r])
AC_FUNC_STRERROR_R

# Set install paths.
if test "x$prefix" = "xNONE" ; then
  prefix="/usr/local"
fi

if test "x$exec_prefix" = "xNONE" ; then
  exec_prefix=$prefix
fi

# Checks for libcurl
if test -d "curl"; then
   ac_configure_args="$ac_configure_args --disable-man-pages --disable-manual --disable-debug --disable-curldebug --bindir=$prefix/lib/$PACKAGE --includedir=$prefix/lib/$PACKAGE/include --datadir=$prefix/lib/$PACKAGE --libdir=$prefix/lib/$PACKAGE"
   AC_CONFIG_SUBDIRS([curl])
else
   AC_MSG_ERROR([libcurl directory not found. please do git submodule init; git submodule update;])
fi

# Configure for SpiderMonkey
ac_configure_args="$ac_configure_args --disable-man-pages --disable-manual --disable-debug --bindir=$prefix/lib/$PACKAGE --includedir=$prefix/lib/$PACKAGE/include --datadir=$prefix/lib/$PACKAGE --libdir=$prefix/lib/$PACKAGE"
AC_CONFIG_SUBDIRS([js-1.5-rc6a])

# Setting for generate Makefile location
AC_CONFIG_FILES([Makefile
                 libwxnkf/Makefile
		 libwxuuid/Makefile
		 sqlite3/Makefile
		 wxsqlite3/Makefile
                 curlpp/Makefile
                 curlpp/src/Makefile
                 curlpp/src/curlpp/Makefile
                 curlpp/src/curlpp/internal/Makefile
                 curlpp/src/utilspp/Makefile
                 curlpp/include/Makefile
                 curlpp/include/curlpp/Makefile
                 curlpp/include/curlpp/internal/Makefile
                 curlpp/include/utilspp/Makefile
                 curlpp/include/utilspp/functor/Makefile
                 curlpp/include/utilspp/singleton/Makefile
                 src/Makefile
		 test/Makefile])
AC_OUTPUT
