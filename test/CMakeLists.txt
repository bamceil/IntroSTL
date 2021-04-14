project(test)

cmake_minimum_required(VERSION 3.12)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
find_package(GTest CONFIG REQUIRED)

macro(build_test arg)
    add_executable(test_${arg} ${arg}_test.cpp)
    target_link_libraries(test_${arg} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
endmacro(build_test arg)

build_test(unique_ptr)
build_test(shared_ptr)
build_test(weak_ptr)
build_test(array)
build_test(vector)
