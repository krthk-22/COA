cmake_minimum_required(VERSION 3.18)

project(coa CXX)

set (CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

add_executable(main src/main.cpp)