cmake_minimum_required(VERSION 2.6.4)

include(${CMAKE_SOURCE_DIR}/runtime.cmake)

file(GLOB_RECURSE SRC *.cpp *.h)

add_executable(
  VMtranslator-all
  ${SRC}
)