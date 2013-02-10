bin_PROGRAMS      = JaneClone
JaneClone_SOURCES = extractboardlist.hpp        \
		    main.cpp                    \
		    socketcommunication.cpp     \
		    threadcontentwindow.hpp     \
		    anchoredresponsepopup.cpp   \
		    janeclone.cpp               \
		    metakitaccessor.cpp         \
		    socketcommunication.hpp     \
		    virtualboardlistctrl.cpp    \
		    anchoredresponsepopup.hpp   \
		    janeclone.hpp               \
		    metakitaccessor.hpp         \
		    threadcontentbar.cpp        \
		    virtualboardlistctrl.hpp    \
		    datatype.hpp                \
		    janecloneutil.cpp           \
		    responsewindow.cpp          \
		    threadcontentbar.hpp        \
		    extractboardlist.cpp        \
		    janecloneutil.hpp           \
		    responsewindow.hpp          \
		    threadcontentwindow.cpp

if USE_WINDRES
# Platform is Windows
JaneClone_CXXFLAGS = @CXXFLAGS@ @XML2CFLAGS@ -I../libwxnkf/libwxnkf -I../libwxnkf/include -I../libmk4/include -I../rc
JaneClone_LDADD    = ../libmk4/libmk4/libmk4.la ../libwxnkf/libwxnkf/libwxnkf.la JaneClone_rc.o

JaneClone_rc.o : ../rc/icon.rc
	@WX_RESCOMP@ -i $^ -O coff -o $@

# library for JaneClone
JaneClone_LDFLAGS  = -static `wx-config --version=2.9 --static=yes --libs aui,qa,adv,base,core,html,net` @XML2LIBS@
ACLOCAL_AMFLAGS    = -I m4
else
# Platform is not Windows
JaneClone_CXXFLAGS = @CXXFLAGS@ @XML2CFLAGS@ -I../libwxnkf/libwxnkf -I../libwxnkf/include -I../libmk4/include -I../rc
JaneClone_LDADD    = ../libmk4/libmk4/libmk4.la ../libwxnkf/libwxnkf/libwxnkf.la
JaneClone_LDFLAGS  = -static @LIBS@ @XML2LIBS@
ACLOCAL_AMFLAGS    = -I m4
endif