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

if(${CMAKE_OS_NAME} MATCHES "OSX")
  # Generate Info.plist
  configure_file(${root_SOURCE_DIR}/Info.plist.in ${root_BINARY_DIR}/Info.plist @ONLY)
  # Resources
  file(GLOB RC_FILES ${janeclone_SOURCE_DIR}/rc/*)
  # Binary
  add_executable(janeclone MACOSX_BUNDLE ${janeclone_SRCS} ${RC_FILES})

  set_target_properties(janeclone PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${root_BINARY_DIR}/Info.plist
    RESOURCE "${RC_FILES}")
else()
  add_executable(janeclone ${janeclone_SRCS})
endif()
file(COPY ${root_SOURCE_DIR}/src/rc DESTINATION ${CMAKE_BINARY_DIR}/src)

include_directories(${LIBXML2_INCLUDE_DIRS})
include("${wxWidgets_USE_FILE}")
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${root_SOURCE_DIR}/babel)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(janeclone ${LIBXML2_LIBRARIES})
target_link_libraries(janeclone ${wxWidgets_LIBRARIES})
target_link_libraries(janeclone ${ZLIB_LIBRARIES})
target_link_libraries(janeclone ${OPENSSL_LIBRARIES})
target_link_libraries(janeclone ${Boost_LIBRARIES} -ldl)

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
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxbase3.0-0v5, libwxgtk3.0-0v5")

  set(JANECLONE_EXEC_RC_DIR "/usr/share/pixmaps/janeclone")

  install(TARGETS janeclone RUNTIME DESTINATION "/usr/bin/")
  file(GLOB RC_FILES ${janeclone_SOURCE_DIR}/rc/*)
  install(FILES ${RC_FILES} DESTINATION ${JANECLONE_EXEC_RC_DIR})

  configure_file(${root_SOURCE_DIR}/misc/janeclone.desktop.in
    ${CMAKE_BINARY_DIR}/misc/janeclone.desktop @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/misc/janeclone.desktop
    DESTINATION /usr/share/applications/)

  # TODO: /usr/lib/janeclone のほうがいいかもしれない
  install(TARGETS wxsqlite3 LIBRARY DESTINATION "/usr/lib/")

  include(CPack)
endif()

#
# Mac用Bundleパッケージの設定
#
if(${CMAKE_OS_NAME} MATCHES "OSX")

  set(CPACK_DMG_FORMAT "UDBZ")
  set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}-osx64-${PACKAGE_VERSION}")
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-osx64-${PACKAGE_VERSION}")
  set(CPACK_PACKAGE_ICON "${root_SOURCE_DIR}/src/rc/janeclone.icns")
  set(CPACK_SYSTEM_NAME "OSX")

  # Note Mac specific extension .app
  set(BUNDLE "\${CMAKE_INSTALL_PREFIX}/janeclone.app")

  # Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
  install(TARGETS janeclone
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime)

  # Directories to look for dependencies
  set(BUNDLE_LIB_DIRS
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/wxsqlite3)

  # ref: https://gitlab.kitware.com/cmake/cmake/-/issues/19663
  install(CODE "include(BundleUtilities)
    set(BU_CHMOD_BUNDLE_ITEMS TRUE)
    fixup_bundle(\"${BUNDLE}\" \"\" \"${BUNDLE_LIB_DIRS}\")")

  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_BUNDLE_NAME "JaneClone")
  set(CPACK_BUNDLE_ICON "${root_SOURCE_DIR}/src/rc/janeclone.icns")
  set(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Info.plist")

  include(CPack)
endif()
