cmake_minimum_required(VERSION 3.15.0)
project(tester)

find_package(GTest REQUIRED)
if (NOT GTEST_FOUND)
    message(WARNING "Could not find GTest, install it manually")
endif()

add_executable(${PROJECT_NAME} tests.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Threads::Threads
        gtest
        gtest_main
)
