cmake_minimum_required(VERSION 3.13)
project(Socket VERSION 0.1 LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE src include/Socket)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_sources(${PROJECT_NAME} PRIVATE
    include/Socket/Exception.h
    include/Socket/Server.h
    include/Socket/Socket.h
)

target_sources(${PROJECT_NAME} PRIVATE
    src/Server.cpp
    src/Socket.cpp
)
