cmake_minimum_required(VERSION 3.10)
project(QOI CXX)

set(PROJECT_ROOT_DIR "${CMAKE_SOURCE_DIR}/..")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIR}/bin") # static libs (.a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIR}/bin") # shared libs (.so/.dll)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_ROOT_DIR}/bin") # executables

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "${PROJECT_ROOT_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT_DIR}/headers)
