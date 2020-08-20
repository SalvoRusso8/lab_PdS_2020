cmake_minimum_required(VERSION 3.16)
project(esercitazione4)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(esercitazione4 main.cpp jobs.h)