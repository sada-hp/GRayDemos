cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
project(DemoPrj)

set(COMPILE_PATH ${DemoPrj_SOURCE_DIR}/bin)
add_subdirectory(engine)
add_subdirectory(demo)