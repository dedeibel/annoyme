
cmake_minimum_required(VERSION 2.6)
project(annoyme C CXX)

ENABLE_TESTING()
SET(UPDATE_TYPE git)
include(CTest)

# Set the variable where all cmake source files and modules are
SET(CMAKE_FILES_DIR "${CMAKE_SOURCE_DIR}/cmake/")
# Add the module include path for custom lib finders
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_FILES_DIR}/Modules/")
# Set the output path to the current build directory
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory("${CMAKE_SOURCE_DIR}/src")

