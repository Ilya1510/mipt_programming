cmake_minimum_required(VERSION 3.6)
project(BigDecart)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Tree.h)
add_executable(BigDecart ${SOURCE_FILES} Tree.h)