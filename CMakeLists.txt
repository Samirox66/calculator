cmake_minimum_required(VERSION 3.8)

project(calcualtor)

add_executable(calc main.cpp Calculator.cpp Calculator.h Operation.h interfaces/interfaces.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET calc PROPERTY CXX_STANDARD 20)
endif()