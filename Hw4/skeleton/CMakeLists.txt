cmake_minimum_required(VERSION 3.10.2)
project(skeleton)

set(CMAKE_CXX_STANDARD 11)

add_executable(skeleton main.cpp Barrier.cpp ThreadSafeList.h)