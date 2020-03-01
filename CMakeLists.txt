cmake_minimum_required(VERSION 3.6)

project (googletest-sample-cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE CXX FLAGS "{CMAKE CXX FLAGS} -Wall -Wextra -Wshadow -ftrapv")

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
endif(APPLE)

# switch DLL or static libary by specifying by command line
set (LIB_TYPE STATIC)
if (BUILD_SHARED_LIBS)
  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
  set (LIB_TYPE SHARED)
endif (BUILD_SHARED_LIBS)

# set Target Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(googletest)
add_subdirectory(src)
add_subdirectory(tests)

# set Startup Project
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "tests-custom" )

# turn on solution folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# create solution folder
set_target_properties(gmock       PROPERTIES FOLDER GoogleTest)
set_target_properties(gmock_main  PROPERTIES FOLDER GoogleTest)
set_target_properties(gtest       PROPERTIES FOLDER GoogleTest)
set_target_properties(gtest_main  PROPERTIES FOLDER GoogleTest)
set_target_properties(calc        PROPERTIES FOLDER library)
set_target_properties(tests-custom PROPERTIES FOLDER executable)
set_target_properties(tests-shared PROPERTIES FOLDER executable)
