cmake_minimum_required(VERSION 3.8)

project(calcualtor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main.cpp Calculator.cpp Calculator.h Operation.h interfaces/interfaces.h)