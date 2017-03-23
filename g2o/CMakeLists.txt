project(g2o)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)


find_package( OpenCV REQUIRED )

add_executable(g2o main.cpp)

target_link_libraries(g2o  g2o_core g2o_types_slam3d g2o_solver_csparse g2o_stuff g2o_csparse_extension g2o_types_sba
${OpenCV_LIBS} )



