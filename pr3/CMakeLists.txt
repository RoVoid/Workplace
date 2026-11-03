cmake_minimum_required(VERSION 3.10)
project(pr3)

set(CMAKE_CXX_STANDARD 20)

add_library(work STATIC src/work.cpp)
target_include_directories(work PUBLIC include)

add_executable(pr3 main.cpp)
target_link_libraries(pr3 PRIVATE work)

