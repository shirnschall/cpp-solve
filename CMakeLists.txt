cmake_minimum_required(VERSION 3.17)
project(solve)

set(CMAKE_CXX_STANDARD 14)

add_library(solve solve.cpp solve.h vector.cpp vector.h)