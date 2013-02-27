ACLOCAL_AMFLAGS = -I m4
noinst_LIBRARIES = libmk4.a
# Sources for libmk4
libmk4_a_SOURCES = column.cpp   \
	   	    custom.cpp   \
		    derived.cpp  \
		    field.cpp    \
		    fileio.cpp   \
		    format.cpp   \
		    handler.cpp  \
		    persist.cpp  \
		    remap.cpp    \
		    std.cpp      \
		    store.cpp    \
		    string.cpp   \
		    table.cpp    \
		    univ.cpp     \
		    view.cpp     \
		    viewx.cpp
# Linker options
libmk4_a_LDFLAG = -static
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmk4_a_CPPFLAGS = -I$(top_builddir)/libmk4