cmake_minimum_required(VERSION 3.29)
project(urfd)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(urfd
    src/config.h
    src/FreeRTOSConfig.h
    src/main.cpp
    src/main.h)