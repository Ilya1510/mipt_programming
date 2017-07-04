cmake_minimum_required(VERSION 3.7)
project(client)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c ../server/workWithMessages.h)
add_executable(client ${SOURCE_FILES})

target_link_libraries(client pthread)