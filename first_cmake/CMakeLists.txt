cmake_minimum_required(VERSION 3.10)
project(first_cmake)

set(CMAKE_CXX_STANDARD 20)

add_library(money STATIC src/money.cpp)
target_include_directories(money PUBLIC include)

add_library(person STATIC src/person.cpp)
target_include_directories(person PUBLIC include)

add_executable(first_cmake main.cpp)
target_link_libraries(first_cmake PRIVATE money person)

