cmake_minimum_required(VERSION 3.7)
project(tuple)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp tuple.h)
add_executable(tuple ${SOURCE_FILES})