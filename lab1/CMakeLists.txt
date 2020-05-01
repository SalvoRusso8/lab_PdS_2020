cmake_minimum_required(VERSION 3.14)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 main.cpp cmake-build-debug/Message.cpp cmake-build-debug/Message.h cmake-build-debug/DurationLogger.cpp cmake-build-debug/DurationLogger.h cmake-build-debug/MessageStore.cpp cmake-build-debug/MessageStore.h)