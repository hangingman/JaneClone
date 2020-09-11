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
    internal/CurlHandle.cpp
    internal/OptionList.cpp
    internal/OptionSetter.cpp
    internal/SList.cpp)

add_library(curlpp SHARED ${curlpp_SRCS})
target_link_libraries(curlpp ${CURL_LIBRARIES})
target_link_libraries(curlpp utilspp)
