cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(janeclone_test CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include("${wxWidgets_USE_FILE}")
include_directories(${janeclone_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

add_executable(sqlite-test sqlite-test.cpp
  ${janeclone_SOURCE_DIR}/sqliteaccessor.cpp)

target_link_libraries(sqlite-test gtest gtest_main pthread)

if(BUILDING_SQLite3)
  include_directories(${root_SOURCE_DIR}/sqlite3)
  target_link_libraries(sqlite-test sqlite3)
else()
  include_directories(${SQLite3_INCLUDE_DIRS})
  target_link_libraries(sqlite-test ${SQLite3_LIBRARIES})
endif()

if(BUILDING_WxSQLite3)
  include_directories(${root_SOURCE_DIR}/wxsqlite3)
  target_link_libraries(sqlite-test wxsqlite3)
else()
  include_directories(${WxSQLite3_INCLUDE_DIRS})
  target_link_libraries(sqlite-test ${WxSQLite3_LIBRARIES})
endif()
			 
add_test(NAME sqlite COMMAND $<TARGET_FILE:sqlite-test>)
set_tests_properties(sqlite PROPERTIES
  ENVIRONMENT WK_DIR=${root_BINARY_DIR}/test)
