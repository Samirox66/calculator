cmake_minimum_require(VERSION 3.8)

project(calcualtor)

add_executable(main.cpp Calculator.cpp Calculator.h Operation.h interfaces/interfaces.h)