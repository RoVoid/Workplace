cmake_minimum_required(VERSION 3.10)
project(pr2)

set(CMAKE_CXX_STANDARD 20)

add_library(work STATIC src/work.cpp)
target_include_directories(work PUBLIC include)

add_executable(pr2 main.cpp)
target_link_libraries(pr2 PRIVATE work)

