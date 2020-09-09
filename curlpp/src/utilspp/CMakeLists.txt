include_directories(${CURL_INCLUDE_DIRS} ${curlpp_SOURCE_DIR}/include
                    ${CMAKE_BINARY_DIR}/curlpp/include)

set(utilspp_SRCS LifetimeLibrary.cpp PrivateMembers.cpp)

add_library(utilspp SHARED ${utilspp_SRCS})
