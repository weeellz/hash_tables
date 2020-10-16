cmake_minimum_required(VERSION 3.1)

project(hash_tables)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)

include_directories(include)

enable_testing()
# add executables
add_executable(test_first tests/d_linked_list.cpp)
# add tests
add_test(test_d_linked_list test_first)
