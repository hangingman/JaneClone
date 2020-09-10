project(sqlite3 C)

add_definitions(-DSQLITE_ENABLE_FTS3)
add_definitions(-DSQLITE_ENABLE_RTREE)

set(sqlite3_HEADERS sqlite3.h sqlite3ext.h)
set(sqlite3_SRCS sqlite3.c)
add_library(sqlite3 SHARED ${sqlite3_SRCS})

include_directories(${root_SOURCE_DIR}/sqlite3)
