cmake_minimum_required(VERSION 3.13)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp big_integer.cpp big_integer.h big_integer_constructors.cpp big_integer_comparators.cpp big_integer_internal.cpp big_integer_arithmetics.cpp big_integer_binarics.cpp)