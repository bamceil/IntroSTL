project(test)

cmake_minimum_required(VERSION 3.12)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)

add_executable(test_unique_ptr unique_ptr_test.cpp)
add_executable(test_shared_ptr shared_ptr_test.cpp)
add_executable(test_weak_ptr weak_ptr_test.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(test_unique_ptr PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(test_shared_ptr PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(test_weak_ptr PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
