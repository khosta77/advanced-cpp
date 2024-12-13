cmake_minimum_required(VERSION 3.0)
project(prep-to-control-work-2)

SET(CMAKE_CXX_FLAGS_RELEASE "-std=c++20 -O0 -Wall")

add_executable(bind.out ./project/bind.cpp)
add_executable(function.out ./project/function.cpp)
add_executable(fusion.out ./project/fusion.cpp)
add_executable(sfinae.out ./project/sfinae.cpp)
