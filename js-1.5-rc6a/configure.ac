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

# Does this platform require array notation to assign to a va_list?
# If cross-compiling, we assume va_list is "normal".  If this breaks
# you, set ac_cv_valistisarray=true and maybe define HAVE_VA_LIST_AS_ARRAY
# also just to be sure.
AC_MSG_CHECKING(whether va_list assignments need array notation)
AC_CACHE_VAL(ac_cv_valistisarray,
	[AC_TRY_RUN([#include <stdlib.h>
		     #include <stdarg.h>
		     void foo(int i, ...) {
			va_list ap1, ap2;
			va_start(ap1, i);
			ap2 = ap1;
			if (va_arg(ap2, int) != 123 || va_arg(ap1, int) != 123)
			    { exit(1); }
			va_end(ap1); va_end(ap2);
		     }
		     int main() { foo(0, 123); return(0); }],
		    [ac_cv_valistisarray=false],
		    [ac_cv_valistisarray=true],
		    [ac_cv_valistisarray=false])])

if test "$ac_cv_valistisarray" = true ; then
	AC_DEFINE(HAVE_VA_LIST_AS_ARRAY, 1,
		[Define as 1 if your va_list type is an array])
	AC_MSG_RESULT(yes)
else
       AC_MSG_RESULT(no)
fi


AC_OUTPUT(Makefile src/Makefile)
