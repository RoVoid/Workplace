cmake_minimum_required(VERSION 3.10)
project(dt)

set(CMAKE_CXX_STANDARD 20)

add_executable(dt main.cpp)
target_include_directories(dt PRIVATE include)


