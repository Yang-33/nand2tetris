cmake_minimum_required(VERSION 2.6.4)

include(${CMAKE_SOURCE_DIR}/runtime.cmake)

# define a variable of project name
set( project_name calc)

# define a variable of project name
project (${project_name})

# define a variable SRC with file GLOB
file(GLOB SRC *.cpp *.h)

# define as exports
if (BUILD_SHARED_LIBS)
  add_definitions(-D${project_name}_EXPORTS)
endif (BUILD_SHARED_LIBS)

# add include directory for ${project_name}_Export.h
include_directories(${PROJECT_BINARY_DIR})

# define sources files of an executable
ADD_LIBRARY(${project_name} ${LIB_TYPE} ${SRC} ${PROJECT_BINARY_DIR}/${project_name}_Export.h)

include (GenerateExportHeader)
GENERATE_EXPORT_HEADER( ${project_name}
	BASE_NAME ${project_name}
	EXPORT_MACRO_NAME ${project_name}_EXPORT
	EXPORT_FILE_NAME ${project_name}_Export.h
	STATIC_DEFINE ${project_name}_BUILT_AS_STATIC
)
