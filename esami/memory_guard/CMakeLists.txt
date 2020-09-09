cmake_minimum_required(VERSION 3.17)
project(memory_guard)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(memory_guard main.cpp)