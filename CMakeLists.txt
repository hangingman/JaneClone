cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

file(READ "version.txt" ver)
string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})

project(
  root
  VERSION "${ver_major}.${ver_minor}.${ver_patch}"
  LANGUAGES CXX
  HOMEPAGE_URL "http://hangingman.github.io/JaneClone/"
  DESCRIPTION "plain browser dedicated to read 5ch thread")
set(PACKAGE_NAME "JaneClone" CACHE INTERNAL "")
set(PACKAGE_STRING "JaneClone ${PROJECT_VERSION}" CACHE INTERNAL "")
set(PACKAGE_VERSION "${PROJECT_VERSION}" CACHE INTERNAL "")

# detect OS
include(IdentifyOS)
if(${CMAKE_OS_NAME} MATCHES "OSX")
  set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl")
  set(ENV{HOMEBREW_NO_AUTO_UPDATE} 1)
endif()

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

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(SQLite3)
find_package(WxSQLite3)
find_package(Boost 1.61 REQUIRED COMPONENTS system filesystem)

message("JaneClone version: ${ver_major}.${ver_minor}.${ver_patch}")

add_subdirectory(cpp-netlib)

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

add_subdirectory(src)

if(test)
  enable_testing()
  find_package(GTest REQUIRED)
  add_subdirectory(test)
  message(STATUS "test     :	  on")
else()
  message(STATUS "test     :	  off")
endif()

if(shingetsu)
  add_definitions(-DUSE_SHINGETSU=1)
  message(STATUS "shingetsu:	  on")
else()
  message(STATUS "shingetsu:	  off")
endif()
