cmake_minimum_required(VERSION 4.4)
project(Casher CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Casher main.cpp
        Database.cpp
        Database.h)
