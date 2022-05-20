cmake_minimum_required(VERSION 3.21)
project(hugeint)
project(hugeint2)
project(hugeint2S)

set(CMAKE_CXX_STANDARD 14)

add_executable(hugeint main.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h)
add_executable(hugeint2 main2.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h)
add_executable(hugeint2S main2.cpp)