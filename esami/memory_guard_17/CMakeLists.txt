cmake_minimum_required(VERSION 3.17)
project(memory_guard_17)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(memory_guard_17 main.cpp)