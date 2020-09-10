project(wxsqlite3 CXX)

add_definitions(-DwxUSE_DYNAMIC_SQLITE3_LOAD=0)
add_definitions(-DWXSQLITE3_HAVE_METADATA=0)
add_definitions(-DWXSQLITE3_HAVE_CODEC=0)
add_definitions(-DWXSQLITE3_HAVE_LOAD_EXTENSION=0)

set(wxsqlite3_HEADERS wx/wxsqlite3.h wx/wxsqlite3def.h wx/wxsqlite3dyn.h
                      wx/wxsqlite3opt.h)
set(wxsqlite3_SRCS wxsqlite3.cpp)
add_library(wxsqlite3 SHARED ${wxsqlite3_SRCS})

include("${wxWidgets_USE_FILE}")

if(BUILDING_SQLITE3)
  include_directories(${root_SOURCE_DIR}/sqlite3
                      ${root_SOURCE_DIR}/wxsqlite3)
  target_link_libraries(wxsqlite3 sqlite3)
else()
  include_directories(${SQLite3_INCLUDE_DIRS} ${root_SOURCE_DIR}/wxsqlite3)
  target_link_libraries(wxsqlite3 ${SQLite3_LIBRARIES})
endif()
