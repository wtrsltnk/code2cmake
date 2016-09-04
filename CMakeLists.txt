cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

get_filename_component(NAME ${CMAKE_SOURCE_DIR} NAME)
project(${NAME})

include(files.cmake)

include_directories(libs/system.io/include)

add_executable(${NAME} ${FILES})
