cmake_minimum_required(VERSION 3.17)
project(restack_tests C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS false)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

message(STATUS "Using GCC flags")
set(CMAKE_C_FLAGS "-Wall -pedantic")

elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")

message(STATUS "Using MSVC flags")
set(CMAKE_C_FLAGS "-Wall")

else()

message(STATUS "Using fallback flags")
set(CMAKE_C_FLAGS "-Wall")

endif()


add_library(tests
            tests.c 
            tests.h 
            side_effect.c
            side_effect.h)

add_executable(restack_tests entry.c)

add_subdirectory(restack)

add_library(cutest external/cutest/CuTest.h external/cutest/CuTest.c)

include_directories(external)

target_link_libraries(restack_tests cutest)

target_link_libraries(restack_tests tests)

target_link_libraries(restack_tests restack)
