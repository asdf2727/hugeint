cmake_minimum_required(VERSION 3.21)
project(hugeint)

set(CMAKE_CXX_STANDARD 14)

add_executable(hugeint main.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h)