# ~ finds wxSQLite3

find_path(
  WxSQLite3_INCLUDE_DIR wx/wxsqlite3.h
  PATHS /usr /usr/local
  PATH_SUFFIXES include)
find_library(
  WxSQLite3_LIBRARY
  NAMES wxsqlite3
  PATHS /usr /usr/local
  PATH_SUFFIXES lib)
mark_as_advanced(WxSQLite3_INCLUDE_DIR WxSQLite3_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WxSQLite3 REQUIRED_VARS WxSQLite3_INCLUDE_DIR
                                                          WxSQLite3_LIBRARY)

if(WxSQLite3_FOUND AND NOT TARGET WxSQLite3::WxSQLite3)
  add_library(WxSQLite3::WxSQLite3 UNKNOWN IMPORTED)
  set_target_properties(
    WxSQLite3::WxSQLite3
    PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
               IMPORTED_LOCATION "${WxSQLite3_LIBRARY}"
               INTERFACE_INCLUDE_DIRECTORIES "${WxSQLite3_INCLUDE_DIR}")
endif()
