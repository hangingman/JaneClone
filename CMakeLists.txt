message(STATUS "Entering directory .")
cmake_minimum_required (VERSION 2.8)
project(JaneClone 1.1.4 CXX)

add_subdirectory(crossguid)
add_subdirectory(sqlite3)
add_subdirectory(wxsqlite3)
add_subdirectory(curlpp)
add_subdirectory(src)
add_subdirectory(test)

