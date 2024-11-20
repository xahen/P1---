cmake_minimum_required(VERSION 3.14)

project(P1_G7_packagedelivery C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)