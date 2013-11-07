#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([js], [1.5.6], [newserver002@gmail.com])
AC_CANONICAL_TARGET
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(SpiderMonkey, 1.5-rc6a)
AM_CONFIG_HEADER([src/jsautotoolscfg.h])

# Checks for programs
AC_PROG_CC
AM_PROG_CC_C_O
AC_EXEEXT
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for HostOS
case "$host" in
        *-*-mingw*)
# MinGW have no localtime_r, so use localtime.
		AC_DEFINE([XP_WIN], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([_BSD_SOURCE], [1], [description])
		AC_DEFINE([POSIX_SOURCE], [1], [description])
		AC_CHECK_LIB(m,asin,LIBS="$LIBS -lm", AC_MSG_ERROR("ERROR: cannot find libm."))
		OS_CONFIG_DONE="1"
		;;
	*-*-cygwin*)
		AC_DEFINE([XP_UNIX], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([_BSD_SOURCE], [1], [description])
		AC_DEFINE([POSIX_SOURCE], [1], [description])
		AC_DEFINE([HAVE_LOCALTIME_R], [1], [description])
		AC_CHECK_LIB(m,asin,LIBS="$LIBS -lm", AC_MSG_ERROR("ERROR: cannot find libm."))
		OS_CONFIG_DONE="1"
                ;;
        *-*-linux*)
		AC_DEFINE([XP_UNIX], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([_BSD_SOURCE], [1], [description])
		AC_DEFINE([POSIX_SOURCE], [1], [description])
		AC_DEFINE([HAVE_LOCALTIME_R], [1], [description])
		AC_CHECK_LIB(m,asin,LIBS="$LIBS -lm", AC_MSG_ERROR("ERROR: cannot find libm."))
		OS_CONFIG_DONE="1"
                ;;
        *-*-darwin*)
		AC_DEFINE([XP_UNIX], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([_BSD_SOURCE], [1], [description])
		AC_DEFINE([POSIX_SOURCE], [1], [description])
		AC_DEFINE([DARWIN], [1], [description])
		OS_LIBS="-lc -framework System"
		JS_EDITLINE="1"
		OS_CONFIG_DONE="1"
                ;;
	*-*-solaris*)
		AC_DEFINE([XP_UNIX], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([SOLARIS], [1], [description])
		AC_DEFINE([HAVE_LOCALTIME_R], [1], [description])
		OS_LIBS="-lsocket -lnsl -ldl -lm"
		HAVE_PURIFY="1"
		JS_EDITLINE="1"
		OS_CONFIG_DONE="1"
		;;
	*-*-freebsd*)
		AC_DEFINE([XP_UNIX], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([_BSD_SOURCE], [1], [description])
		AC_DEFINE([POSIX_SOURCE], [1], [description])
		AC_CHECK_LIB(m,asin,LIBS="$LIBS -lm", AC_MSG_ERROR("ERROR: cannot find libm."))
		OS_CONFIG_DONE="1"
		;;
        *)
		AC_MSG_WARN([*** Please add $host to configure.ac checks!])
		;;
esac

# Setting for libraries needed each OS
if test "$OS_CONFIG_DONE" = '1'; then
    AC_MSG_RESULT([$OS_CONFIG])
    AC_SUBST(LIBS)
else
    AC_MSG_ERROR([unknown. $OS_CONFIG])
fi

dnl Checking for va_copy availability
AC_MSG_CHECKING([for va_copy])
AC_TRY_LINK([#include <stdarg.h>
va_list ap1,ap2;], [va_copy(ap1,ap2);],
have_va_copy=yes,
have_va_copy=no)
AC_MSG_RESULT($have_va_copy)
if test x"$have_va_copy" = x"yes"; then
    AC_DEFINE(HAVE_VA_COPY,1,[Whether va_copy() is available])
else
    AC_MSG_CHECKING([for __va_copy])
    AC_TRY_LINK([#include <stdarg.h>
    va_list ap1,ap2;], [__va_copy(ap1,ap2);],
    have___va_copy=yes,
    have___va_copy=no)
    AC_MSG_RESULT($have___va_copy)
    if test x"$have___va_copy" = x"yes"; then
        AC_DEFINE(HAVE___VA_COPY,1,[Whether __va_copy() is available])
    fi
fi

AC_OUTPUT(Makefile src/Makefile)
