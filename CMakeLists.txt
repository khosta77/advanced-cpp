cmake_minimum_required(VERSION 3.0)
project(HWx)

# Выставить версию с++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Добавление исполняемого файла с именем main.out
add_executable(
        main.out
        ./project/main.cpp
        #./project/test.cpp
    )

