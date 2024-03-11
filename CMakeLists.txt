cmake_minimum_required(VERSION 3.22)
project(rickv_generator)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(google_tests)

add_executable(rickv_generator MyRV32_model.h main.cpp)
