#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT(SpiderMonkey, 1.5-rc6a, newserver002@gmail.com)
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
        *-*-mingw*|*-*-cygwin*)
                ;;
        *-*-linux*)
		AC_DEFINE([XP_UNIX], [1], [description])
		AC_DEFINE([SVR4], [1], [description])
		AC_DEFINE([SYSV], [1], [description])
		AC_DEFINE([_BSD_SOURCE], [1], [description])
		AC_DEFINE([POSIX_SOURCE], [1], [description])
		AC_DEFINE([HAVE_LOCALTIME_R], [1], [description])
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
		OS_LIBS="-lm"
		OS_CONFIG_DONE="1"
		;;
        *)
		AC_MSG_WARN([*** Please add $host to configure.ac checks!])
		;;
esac

# Setting for libraries needed each OS
if test "$OS_CONFIG_DONE" = '1'; then
    AC_MSG_RESULT([$OS_CONFIG])
    AC_SUBST(OS_LIBS)
else
    AC_MSG_ERROR([unknown. $OS_CONFIG])
fi

AC_OUTPUT(Makefile src/Makefile)
