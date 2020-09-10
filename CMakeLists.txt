cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(
  root
  VERSION 1.1.6
  LANGUAGES CXX
  HOMEPAGE_URL "http://hangingman.github.io/JaneClone/"
  DESCRIPTION "plain browser dedicated to read 5ch thread")

include(IdentifyOS)
add_definitions(-DPACKAGE_VERSION="${PROJECT_VERSION}")
add_definitions(-DHAVE_CONFIG_H)
configure_file(config.h.in config.h @ONLY)

set(UPSTREAM_URL
    "https://github.com/hangingman/JaneClone"
    CACHE INTERNAL "")
set(PACKAGE_BUGREPORT
    "idiotpanzer@gmail.com"
    CACHE INTERNAL "")
set(DEB_STANDARDS_VERSION
    3.9.3
    CACHE INTERNAL "")

find_package(LibXml2 REQUIRED)
find_package(
  wxWidgets
  COMPONENTS aui
             qa
             adv
             base
             core
             html
             net
             xrc
  REQUIRED)
set(wxWidgets_CONFIG_OPTIONS --toolkit=aui,qa,adv,base,core,html,net,xrc)

find_package(CURL REQUIRED)
find_package(CURLpp)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3)
find_package(WxSQLite3)

if(NOT CURLpp_FOUND)
  add_subdirectory(curlpp)
  option(BUILDING_CURLPP "Building CURLpp" ON)
endif()
if(NOT SQLite3_FOUND)
  add_subdirectory(sqlite3)
  option(BUILDING_SQLite3 "Building SQLite3" ON)
endif()
if(NOT WxSQLite3_FOUND)
  add_subdirectory(wxsqlite3)
  option(BUILDING_WxSQLite3 "Building WxSQLite3" ON)
endif()

add_subdirectory(src)

#
# Debianパッケージの設定
#
if(${CMAKE_OS_NAME} MATCHES "(Debian|Ubuntu)")
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Hiroyuki Nagata")
  set(CPACK_PACKAGE_NAME "janeclone")
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxbase3.0, libwxgtk3.0")

  install(TARGETS janeclone RUNTIME DESTINATION "/usr/bin/")
  file(GLOB RC_FILES ${janeclone_SOURCE_DIR}/rc/*)
  install(FILES ${RC_FILES} DESTINATION /usr/share/pixmaps/janeclone/)

  include(CPack)
endif()
