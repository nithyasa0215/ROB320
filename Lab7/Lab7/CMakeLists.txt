cmake_minimum_required(VERSION 3.10)
project(Lab7)
set(CMAKE_CXX_STANDARD 20)

find_library(PTHREAD pthread REQUIRED)

# Define libraries
set(RIX_LIBS
    ${CMAKE_SOURCE_DIR}/lib/librixcore.a
    ${CMAKE_SOURCE_DIR}/lib/librixutil.a
    ${CMAKE_SOURCE_DIR}/lib/librixipc.a
)

# Gradient descent service.
add_executable(gradient_descent_service gradient_descent_service.cpp)
target_include_directories(gradient_descent_service PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gradient_descent_service PUBLIC ${RIX_LIBS} pthread)

# Gradient descent service client.
add_executable(gradient_descent_client gradient_descent_client.cpp)
target_include_directories(gradient_descent_client PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gradient_descent_client PUBLIC ${RIX_LIBS} pthread)