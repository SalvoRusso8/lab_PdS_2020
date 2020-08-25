cmake_minimum_required(VERSION 3.17)
project(lab5)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(lab5 main.cpp ClientConn.cpp ClientConn.h ChatServer.cpp ChatServer.h)