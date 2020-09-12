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

if(NOT SQLite3_FOUND)
  add_subdirectory(sqlite3)
  option(BUILDING_SQLite3 "Building SQLite3" ON)
  message(STATUS "sqlite3  :	  builtin")
else()
  message(STATUS "sqlite3  :	  sys")
endif()

if(NOT WxSQLite3_FOUND)
  add_subdirectory(wxsqlite3)
  option(BUILDING_WxSQLite3 "Building WxSQLite3" ON)
  message(STATUS "wxsqlite3:	  builtin")
else()
  message(STATUS "wxsqlite3:	  sys")
endif()

if(NOT CURLpp_FOUND)
  add_subdirectory(curlpp)
  option(BUILDING_CURLPP "Building CURLpp" ON)
  message(STATUS "curlpp   :	  builtin")
else()
  message(STATUS "curlpp   :	  sys")
endif()

add_subdirectory(src)

if(test)
  enable_testing()
  find_package(GTest REQUIRED)
  add_subdirectory(test)
  message(STATUS "test     :	  on")
else()
  message(STATUS "test     :	  off")
endif()
