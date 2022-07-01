cmake_minimum_required(VERSION 3.18)
project(calculator)

set(CMAKE_CXX_STANDARD 11)

add_executable(calculator arithmetic.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h hugeint_extra.cpp)