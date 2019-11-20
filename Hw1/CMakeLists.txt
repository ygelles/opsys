cmake_minimum_required(VERSION 3.10)
project(Smash)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
add_executable(Smash smash.cpp Commands.h Commands.cpp signals.h signals.cpp defines.h)