cmake_minimum_required(VERSION 3.0)
project(HWx)

# Выставить версию с++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O0 -Wall")

add_executable(bind.out ./project/bind.cpp)
add_executable(function.out ./project/function.cpp)
add_executable(fusion.out ./project/fusion.cpp)
add_executable(sfinae.out ./project/sfinae.cpp)
