cmake_minimum_required(VERSION 3.17)
project(restack_tests C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS false)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

message(STATUS "Using GCC flags")
set(CMAKE_C_FLAGS "-Wall -pedantic")

elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")

message(STATUS "Using MSVC flags")
set(CMAKE_C_FLAGS "-Wall -pedantic")

else()

message(STATUS "Using fallback flags")
set(CMAKE_C_FLAGS "-Wall")

endif()


add_executable(restack_tests test.c)

add_subdirectory(restack)

add_library(munit external/munit/munit.h external/munit/munit.c)

include_directories(external)

target_link_libraries(restack_tests munit)

target_link_libraries(restack_tests restack)
