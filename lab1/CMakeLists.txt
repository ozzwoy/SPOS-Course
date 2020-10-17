cmake_minimum_required(VERSION 3.17)
project(SPOS_Lab_1)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(BOOST_ALL_NO_LIB)

set(BOOST_ROOT ${PROJECT_SOURCE_DIR}/boost_1_74_0)

find_package(Boost 1.74.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(SPOS_Lab_1 main.cpp)
target_link_libraries(SPOS_Lab_1 ${Boost_LIBRARIES})