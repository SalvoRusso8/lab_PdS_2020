cmake_minimum_required(VERSION 3.17)
project(socket_test)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(socket_test main.cpp)