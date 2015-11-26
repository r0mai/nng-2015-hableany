cmake_minimum_required(VERSION 2.8)

project(nng-2015-hableany)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -O0")
endif()

aux_source_directory(bin BIN_SOURCES)
aux_source_directory(src SRC_SOURCES)
aux_source_directory(test TEST_SOURCES)

add_library(libnng STATIC ${SRC_SOURCES})

add_executable(nng ${BIN_SOURCES})
target_link_libraries(nng libnng)

add_executable(test-nng ${TEST_SOURCES})
target_link_libraries(test-nng libnng)
