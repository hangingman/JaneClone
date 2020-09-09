cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(janeclone_bin CXX)

set(janeclone_bin_SRCS
    anchoredresponsepopup.cpp
    anchoredresponsepopup.hpp
    datatype.hpp
    drawpane.cpp
    drawpane.hpp
    enums.hpp
    enumstring.hpp
    extractboardlist.cpp
    extractboardlist.hpp
    imagepanel.cpp
    imagepanel.hpp
    janeclone.cpp
    janeclone.hpp
    janecloneimageviewer.cpp
    janecloneimageviewer.hpp
    janecloneuiutil.cpp
    janecloneuiutil.hpp
    janecloneutil.cpp
    janecloneutil.hpp
    main.cpp
    otherpanels.cpp
    otherpanels.hpp
    responsewindow.cpp
    responsewindow.hpp
    settingwindow.cpp
    settingwindow.hpp
    socketcommunication.cpp
    socketcommunication.hpp
    sqliteaccessor.cpp
    sqliteaccessor.hpp
    threadcontentbar.cpp
    threadcontentbar.hpp
    threadcontentwindow.cpp
    threadcontentwindow.hpp
    virtualboardlistctrl.cpp
    virtualboardlistctrl.hpp
    ../babel/babel.cpp
    ../babel/babel.h)

add_executable(janeclone_bin ${janeclone_bin_SRCS})
file(COPY ${janeclone_SOURCE_DIR}/src/rc DESTINATION ${CMAKE_BINARY_DIR}/src)

include_directories(${LIBXML2_INCLUDE_DIRS})
include("${wxWidgets_USE_FILE}")
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${janeclone_SOURCE_DIR}/babel)
include_directories(${CMAKE_BINARY_DIR})

target_link_libraries(janeclone_bin ${LIBXML2_LIBRARIES})
target_link_libraries(janeclone_bin ${wxWidgets_LIBRARIES})
target_link_libraries(janeclone_bin ${ZLIB_LIBRARIES})
target_link_libraries(janeclone_bin ${CURL_LIBRARIES})
target_link_libraries(janeclone_bin ${OPENSSL_LIBRARIES})

if(BUILDING_CURLPP)
  include_directories(${janeclone_SOURCE_DIR}/curlpp/include)
  include_directories(${CMAKE_BINARY_DIR}/curlpp/include)
  target_link_libraries(janeclone_bin curlpp)
  target_link_libraries(janeclone_bin utilspp)
  target_link_libraries(janeclone_bin internal)
else()
  include_directories(${CURLpp_INCLUDE_DIRS})
  target_link_libraries(janeclone_bin ${CURLpp_LIBRARIES})
endif()

if(BUILDING_SQLite3)
  include_directories(${janeclone_SOURCE_DIR}/sqlite3)
  target_link_libraries(janeclone_bin sqlite3)
else()
  include_directories(${SQLite3_INCLUDE_DIRS})
  target_link_libraries(janeclone_bin ${SQLite3_LIBRARIES})
endif()

if(BUILDING_WxSQLite3)
  include_directories(${janeclone_SOURCE_DIR}/wxsqlite3)
  target_link_libraries(janeclone_bin wxsqlite3)
else()
  include_directories(${WxSQLite3_INCLUDE_DIRS})
  target_link_libraries(janeclone_bin ${WxSQLite3_LIBRARIES})
endif()
