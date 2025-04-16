cmake_minimum_required(VERSION 3.11)
project(Lab5 VERSION 1.0)

include(FetchContent)

FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0
)
FetchContent_MakeAvailable(eigen)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(transforms STATIC transforms.cpp routines.cpp)
target_include_directories(transforms PRIVATE .)
target_link_libraries(transforms PUBLIC Eigen3::Eigen)

add_executable(test_transforms test_transforms.cpp)
target_link_libraries(test_transforms PRIVATE transforms GTest::gtest_main)

add_executable(test_routines test_routines.cpp)
target_link_libraries(test_routines PRIVATE transforms GTest::gtest_main)