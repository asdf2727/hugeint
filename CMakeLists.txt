cmake_minimum_required(VERSION 3.18)
project(calculator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

add_executable(calulator
		arithmetic.cpp
		hugeint_math.cpp
		hugeint_cast.cpp
		hugeint.h)
add_executable(test
		test.cpp
		hugeint_math.cpp
		hugeint_cast.cpp
		hugeint.h)
add_executable(conjecture
		3n+1_Conjecture.cpp
		hugeint_math.cpp
		hugeint_cast.cpp
		hugeint.h)
add_executable(check
		check.cpp
		hugeint_math.cpp
		hugeint_cast.cpp
		hugeint.h)