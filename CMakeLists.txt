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
    src/linuxApi.h
    src/Server.cpp
    src/Socket.cpp
    src/windowsApi.h
)

if (WIN32)
    find_library(ws2_32_LIBRARY_PATH ws2_32 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${ws2_32_LIBRARY_PATH})
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DWIN32)
endif()

if (UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DUNIX)
endif()
