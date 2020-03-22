cmake_minimum_required(VERSION 2.6.4)

include(${CMAKE_SOURCE_DIR}/runtime.cmake)

add_library(
  Calcdir STATIC
  calc.cpp
  sstring.cpp
)