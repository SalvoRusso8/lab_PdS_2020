cmake_minimum_required(VERSION 3.17)
project(es20200721)

set(CMAKE_CXX_STANDARD 17)

add_executable(es20200721 main.cpp)


SET(CMAKE_CXX_FLAGS -pthread)