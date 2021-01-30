cmake_minimum_required(VERSION 3.17)
project(restack_tests C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS false)
set(CMAKE_C_FLAGS "-Wall -pedantic")

add_executable(restack_tests test.c)

add_subdirectory(restack)

target_link_libraries(restack_tests restack)
target_link_libraries(restack_tests cunit)
