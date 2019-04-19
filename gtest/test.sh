#!/bin/bash

g++ -O2 -std=c++11 -c gtest-all.cc -o tmp/_all.o
g++ -O2 -std=c++11 -c gtest_main.cc -o tmp/_main.o
g++ -O2 -std=c++11 -c ../big_integer_testing.cpp -o tmp/_testing.o

g++ -O2 -std=c++11 tmp/*.o ../cmake-build-debug/CMakeFiles/big_integer.dir/big_integer.cpp.o \
                   ../cmake-build-debug/CMakeFiles/big_integer.dir/engine/_asm_vector.asm.o \
                    -o test
