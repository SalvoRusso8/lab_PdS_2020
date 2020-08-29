cmake_minimum_required(VERSION 3.17)
project(PubSubParadigm)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(PubSubParadigm main.cpp PubSubHub.cpp PubSubHub.h Subscription.h)