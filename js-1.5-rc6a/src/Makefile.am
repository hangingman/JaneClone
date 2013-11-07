pkginclude_HEADERS = jsautotoolscfg.h \
                     jsapi.h jsarena.h jsarray.h jsatom.h jsautocfg.h jsbit.h \
                     jsbool.h jsclist.h jscntxt.h jscompat.h jsconfig.h jscpucfg.h \
                     jsdate.h jsdbgapi.h jsdhash.h jsdtoa.h jsemit.h jsexn.h \
                     jsfile.h jsfun.h jsgc.h jshash.h jsinterp.h jslibmath.h \
                     jslock.h jslong.h jsmath.h jsnum.h jsobj.h jsopcode.h jsosdep.h \
                     jsotypes.h jsparse.h jsprf.h jsprvtd.h jspubtd.h jsregexp.h \
                     jsscan.h jsscope.h jsscript.h jsstddef.h jsstr.h jstypes.h \
                     jsutil.h jsxdrapi.h
noinst_HEADERS = resource.h prmjtime.h

noinst_LTLIBRARIES = libjs.la
libjs_la_LIBADD    = @LIBS@
libjs_la_SOURCES   = \
jsapi.c \
jsarena.c \
jsarray.c \
jsatom.c \
jsbool.c \
jscntxt.c \
jsdate.c \
jsdbgapi.c \
jsdhash.c \
jsdtoa.c \
jsemit.c \
jsexn.c \
jsfile.c \
jsfun.c \
jsgc.c \
jshash.c \
jsinterp.c \
jslock.c \
jslog2.c \
jslong.c \
jsmath.c \
jsnum.c \
jsobj.c \
jsopcode.c \
jsparse.c \
jsprf.c \
jsregexp.c \
jsscan.c \
jsscope.c \
jsscript.c \
jsstr.c \
jsutil.c \
jsxdrapi.c \
prmjtime.c \
jsapi.h \
jsarena.h \
jsarray.h \
jsatom.h \
jsbit.h \
jsbool.h \
jsclist.h \
jscntxt.h \
jscompat.h \
jsconfig.h \
jsdate.h \
jsdbgapi.h \
jsdhash.h \
jsdtoa.h \
jsemit.h \
jsexn.h \
jsfile.h \
jsfun.h \
jsgc.h \
jshash.h \
jsinterp.h \
jslibmath.h \
jslock.h \
jslong.h \
jsmath.h \
jsnum.h \
jsobj.h \
jsopcode.h \
jsosdep.h \
jsotypes.h \
jsparse.h \
jsprf.h \
jsprvtd.h \
jspubtd.h \
jsregexp.h \
jsscan.h \
jsscope.h \
jsscript.h \
jsstddef.h \
jsstr.h \
jstypes.h \
jsutil.h \
jsxdrapi.h \
prmjtime.h \
resource.h \
js.msg \
jsopcode.tbl

bin_PROGRAMS = jscpucfg js
jscpucfg_SOURCES = jscpucfg.c jscpucfg.h
jscpucfg_CFLAGS = $(AM_CFLAGS)
jscpucfg_LDADD = @LIBS@

js_SOURCES = js.c jsshell.msg 
js_CFLAGS = $(AM_CFLAGS)
js_LDADD = libjs.la @LIBS@

jsapi.c: jsautocfg.h

js.c: jsautocfg.h

jsautocfg.h: jscpucfg
	-rm -f jsautocfg.h
	./jscpucfg > jsautocfg.h

