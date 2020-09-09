project(curlpp CXX)

# autotools同様のconfig.hを作成する option(FOO_ENABLE "Enable Foo" ON) if(FOO_ENABLE)
# set(FOO_STRING "foo") endif()
configure_file(include/curlpp/config.h.in include/curlpp/config.h @ONLY)
add_definitions(-DHAVE_CONFIG_H)

add_subdirectory(src)
add_subdirectory(include)
