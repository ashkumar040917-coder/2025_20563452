cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# This project contains a library
add_library(maths adder.cpp)

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-Dmaths_STATIC)
endif()

# It also contains an executable
add_executable(calc calc.cpp)

# The executable uses the library
target_link_libraries(calc maths)