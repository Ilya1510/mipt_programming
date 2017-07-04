cmake_minimum_required(VERSION 2.8)
project(grep)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(grep ${SOURCE_FILES})
