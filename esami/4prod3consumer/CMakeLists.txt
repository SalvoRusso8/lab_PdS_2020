cmake_minimum_required(VERSION 3.17)
project(4prod3consumer)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(4prod3consumer main.cpp)