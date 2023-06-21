cmake_minimum_required(VERSION 3.18)
project(arithmetic)
project(test)
project(conjecture)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(arithmetic arithmetic.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h)
add_executable(test test.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h)
add_executable(conjecture 3n+1_Conjecture.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h)