cmake_minimum_required(VERSION 3.1)

# Set project name
project(HTTPServer)

# Set version number of the project
set(HTTPServer_VERSION_MAJOR 0)
set(HTTPServer_VERSION_MINOR 1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify build options
set(CMAKE_BUILD_TYPE Debug)


# Set path to externals libraries
set(EXTERNALS_LIB_PATH ./ExternalsLibs)

# set path to YAML-CPP library
set(YAML_LIB_PATH ${EXTERNALS_LIB_PATH}/yaml-cpp)
set(YAML_INCLUDE_DIR 
${YAML_LIB_PATH}/include
#${YAML_LIB_PATH}/include/yaml-cpp/node
#${YAML_LIB_PATH}/include/yaml-cpp/contrib
)

set(YAML_LIB_DIR ${YAML_LIB_PATH}/build)
find_library(YAML_LIBRARIES NAMES "yaml-cpp" PATHS ${YAML_LIB_DIR})

message("YAML_LIB_DIR = " ${YAML_LIB_DIR})

# Add the includes directories to the project
include_directories(includes)

set(HEADERS 
includes/HTTPServer.h
)

set(SOURCES
sources/HTTPServer.cpp
)

# Setup output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Linux/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Linux/lib)

add_executable(HTTPServer
${HEADERS}
${SOURCES}
main.cpp
)

target_include_directories(HTTPServer PRIVATE ${YAML_INCLUDE_DIR})

target_link_libraries(HTTPServer
${YAML_LIBRARIES}
)
