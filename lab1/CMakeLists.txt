cmake_minimum_required(VERSION 3.17)
project(SPOS_Lab_1)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(_WIN32_WINNT=0x0601)

set(BOOST_ROOT ${PROJECT_SOURCE_DIR}/boost_1_74_0)

find_package(Boost 1.74.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${PROJECT_SOURCE_DIR}/boost_1_74_0/stage/lib)

add_subdirectory(${PROJECT_SOURCE_DIR}/function/function_manager)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})