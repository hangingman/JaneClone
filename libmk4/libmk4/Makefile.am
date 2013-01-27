ACLOCAL_AMFLAGS = -I m4
lib_LTLIBRARIES = libmk4.la
# Sources for libmk4
libmk4_la_SOURCES = column.cpp   \
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
libmk4_la_LDFLAGS = 
# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmk4_la_CPPFLAGS = -I../include