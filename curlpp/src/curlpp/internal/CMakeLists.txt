include_directories(${CURL_INCLUDE_DIRS} ${curlpp_SOURCE_DIR}/include
                    ${CMAKE_BINARY_DIR}/curlpp/include)

set(internal_SRCS CurlHandle.cpp OptionList.cpp OptionSetter.cpp SList.cpp)

add_library(internal SHARED ${internal_SRCS})
