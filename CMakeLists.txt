cmake_minimum_required(VERSION 3.10)

project(socket VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
    Exception.h
    Server.h
    Socket.h
)

set(SOURCE_FILES
    Server.cpp
    Socket.cpp
)

set(INCLUDE_DIRS
    src
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_BINARY_DIR})
