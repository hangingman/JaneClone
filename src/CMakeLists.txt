project(janeclone CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

set(janeclone_SRCS
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

add_executable(janeclone ${janeclone_SRCS})
file(COPY ${root_SOURCE_DIR}/src/rc DESTINATION ${CMAKE_BINARY_DIR}/src)

include_directories(${LIBXML2_INCLUDE_DIRS})
include("${wxWidgets_USE_FILE}")
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${root_SOURCE_DIR}/babel)
include_directories(${CMAKE_BINARY_DIR})

target_link_libraries(janeclone ${LIBXML2_LIBRARIES})
target_link_libraries(janeclone ${wxWidgets_LIBRARIES})
target_link_libraries(janeclone ${ZLIB_LIBRARIES})
target_link_libraries(janeclone ${CURL_LIBRARIES})
target_link_libraries(janeclone ${OPENSSL_LIBRARIES})

if(BUILDING_CURLPP)
  include_directories(${root_SOURCE_DIR}/curlpp/include)
  include_directories(${CMAKE_BINARY_DIR}/curlpp/include)
  target_link_libraries(janeclone curlpp)
  target_link_libraries(janeclone utilspp)
else()
  include_directories(${CURLpp_INCLUDE_DIRS})
  target_link_libraries(janeclone ${CURLpp_LIBRARIES})
endif()

if(BUILDING_SQLite3)
  include_directories(${root_SOURCE_DIR}/sqlite3)
  target_link_libraries(janeclone sqlite3)
else()
  include_directories(${SQLite3_INCLUDE_DIRS})
  target_link_libraries(janeclone ${SQLite3_LIBRARIES})
endif()

if(BUILDING_WxSQLite3)
  include_directories(${root_SOURCE_DIR}/wxsqlite3)
  target_link_libraries(janeclone wxsqlite3)
else()
  include_directories(${WxSQLite3_INCLUDE_DIRS})
  target_link_libraries(janeclone ${WxSQLite3_LIBRARIES})
endif()

#
# Debianパッケージの設定
#
if(${CMAKE_OS_NAME} MATCHES "(Debian|Ubuntu)")
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Hiroyuki Nagata")
  set(CPACK_PACKAGE_NAME "janeclone")
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxbase3.0-0v5, libwxgtk3.0-0v5, libcurl3 | libcurl4")

  set(JANECLONE_EXEC_RC_DIR "/usr/share/pixmaps/janeclone")

  install(TARGETS janeclone RUNTIME DESTINATION "/usr/bin/")
  file(GLOB RC_FILES ${janeclone_SOURCE_DIR}/rc/*)
  install(FILES ${RC_FILES} DESTINATION ${JANECLONE_EXEC_RC_DIR})

  configure_file(${root_SOURCE_DIR}/misc/janeclone.desktop.in
    ${CMAKE_BINARY_DIR}/misc/janeclone.desktop @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/misc/janeclone.desktop
    DESTINATION /usr/share/applications/)

  # TODO: /usr/lib/janeclone のほうがいいかもしれない
  install(TARGETS curlpp LIBRARY DESTINATION "/usr/lib/")
  install(TARGETS utilspp LIBRARY DESTINATION "/usr/lib/")
  install(TARGETS wxsqlite3 LIBRARY DESTINATION "/usr/lib/")

  include(CPack)
endif()
