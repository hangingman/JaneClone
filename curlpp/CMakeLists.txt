project(curlpp CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(include/curlpp/config.h.in include/curlpp/config.h @ONLY)
add_definitions(-DHAVE_CONFIG_H)

add_subdirectory(src)
add_subdirectory(include)
