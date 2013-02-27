#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([JaneClone], [1.0.0], [newserver002@gmail.com])
#AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([libmk4/mk4.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for HostOS
case "$host" in
        *-*-mingw*|*-*-cygwin*)
                AC_DEFINE(PLATFORM_WIN32, 1, [Platform is Win32])
                PLATFORM_STRING="Win32"
		AC_SUBST(PLATFORM_STRING)
                ;;
        *-*-linux*)
                AC_DEFINE(PLATFORM_LINUX, 1, [Platform is Linux])
                PLATFORM_STRING="Linux"
		AC_SUBST(PLATFORM_STRING)
                ;;
        *-*-darwin*)
                AC_DEFINE(PLATFORM_APPLE, 1, [Platform is Apple])
                PLATFORM_STRING="Apple"
		AC_SUBST(PLATFORM_STRING)
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac

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

# Checks for zlib
AC_CHECK_LIB(z, gzopen)

# Add Flags
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h langinfo.h locale.h stdlib.h string.h strings.h unistd.h])

# Checks for long value
AC_CHECK_SIZEOF([long])
if test ac_cv_sizeof_long = 8; then
	AC_DEFINE(SIZEOF_LONG, 8)
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset munmap nl_langinfo strcasecmp strchr strcspn strpbrk strrchr strspn strstr])

AC_CONFIG_FILES([Makefile
                 libmk4/Makefile
                 libwxnkf/Makefile
		 libwxuuid/Makefile
                 src/Makefile])
AC_OUTPUT
