cmake_minimum_required(VERSION 3.12)

set(CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告

project(intro_stl)

set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
aux_source_directory(${CMAKE_SOURCE_DIR}/src proj_src)
add_executable(${PROJECT_NAME} ${proj_src})
add_subdirectory(test)
enable_testing()

macro(do_test arg)
    add_test(test_${arg} ${CMAKE_SOURCE_DIR}/build/test/test_${arg})
endmacro(do_test arg)

do_test(unique_ptr)
do_test(shared_ptr)
do_test(weak_ptr)
do_test(array)
do_test(vector)
