cmake_minimum_required(VERSION 3.8)
project(untitled)
set(CMAKE_CXX_STANDARD 11)
find_package(GTest REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(untitled ${SOURCE_FILES})
target_link_libraries(untitled GTest::GTest)

