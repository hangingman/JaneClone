add_subdirectory(internal)
include_directories(${CURL_INCLUDE_DIRS} ${curlpp_SOURCE_DIR}/include
                    ${CMAKE_BINARY_DIR}/curlpp/include)

set(curlpp_SRCS
    cURLpp.cpp
    Exception.cpp
    Easy.cpp
    Form.cpp
    Info.cpp
    Multi.cpp
    OptionBase.cpp
    Options.cpp
    autolink.h)

add_library(curlpp SHARED ${curlpp_SRCS})
target_link_libraries(curlpp internal)
target_link_libraries(curlpp utilspp)
