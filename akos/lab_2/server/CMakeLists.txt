cmake_minimum_required(VERSION 3.7)
project(server)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c workWithMessages.h)

add_executable(server ${SOURCE_FILES})

target_link_libraries(server pthread)