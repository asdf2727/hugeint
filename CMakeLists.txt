cmake_minimum_required(VERSION 3.18)
project(calculator)
project(ciur)

set(CMAKE_CXX_STANDARD 14)

add_executable(calulator arithmetic_calc.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h hugeint_extra.cpp)
add_executable(ciur ciur.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h hugeint_extra.cpp)