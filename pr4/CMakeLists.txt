cmake_minimum_required(VERSION 3.31)
project(pr4)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(BOOST_INCLUDE_LIBRARIES program_options)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(Boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP ON)
FetchContent_MakeAvailable(Boost)

add_library(work STATIC src/work.cpp)
target_include_directories(work PUBLIC include)

add_executable(pr4 main.cpp)
target_link_libraries(pr4 PRIVATE work)
target_link_libraries(pr4 PRIVATE Boost::program_options)

