cmake_minimum_required(VERSION 3.10)

project(rix-rdf)

set(CMAKE_CXX_STANDARD 20)

if(DEFINED LOGLEVEL)
    add_compile_definitions(RIX_UTIL_LOG_LEVEL=${LOGLEVEL})
else()
    add_compile_definitions(RIX_UTIL_LOG_LEVEL=1)
endif()

find_library(PTHREAD pthread REQUIRED)

add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Define include directories
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/eigen/
    ${CMAKE_SOURCE_DIR}/json/include
)

# Define libraries
set(RIX_LIBS
    ${CMAKE_SOURCE_DIR}/lib/librixcore.a
    ${CMAKE_SOURCE_DIR}/lib/librixutil.a
    ${CMAKE_SOURCE_DIR}/lib/librixipc.a
)

### RIX RDF LIBRARY ###
add_library(rixrdf STATIC
    src/rix/rdf/util.cpp
    src/rix/rdf/link.cpp
    src/rix/rdf/joint.cpp
    src/rix/rdf/tree.cpp
    src/rix/rdf/fk_solver.cpp
)
target_include_directories(rixrdf PUBLIC ${INCLUDE_DIRS})
target_link_libraries(rixrdf PUBLIC ${RIX_LIBS} pthread)

### Joint State Publisher ###
add_executable(jsp src/jsp.cpp)
target_link_libraries(jsp rixrdf)

### Robot State Publisher ###
add_executable(rsp src/rsp.cpp)
target_link_libraries(rsp rixrdf)

### Manual Tests ###

### JSP Simulator ###
add_executable(jsp_simulator test/manual_tests/jsp_simulator.cpp)
target_link_libraries(jsp_simulator rixrdf)

### Joint Publisher ###
add_executable(joint_publisher test/manual_tests/joint_publisher.cpp)
target_link_libraries(joint_publisher rixrdf)

### Joint State Listener ###
add_executable(js_listener test/manual_tests/js_listener.cpp)
target_link_libraries(js_listener rixrdf)

### TF Listener ###
add_executable(tf_listener test/manual_tests/tf_listener.cpp)
target_link_libraries(tf_listener rixrdf)

### Joint State Recorder ###
add_executable(js_recorder test/manual_tests/js_recorder.cpp)
target_link_libraries(js_recorder rixrdf)

### TF Recorder ###
add_executable(tf_recorder test/manual_tests/tf_recorder.cpp)
target_link_libraries(tf_recorder rixrdf)

### Compare JSP Test ###
add_executable(compare_jsp_test test/manual_tests/compare_jsp_test.cpp)
target_link_libraries(compare_jsp_test rixrdf)

### Compare JSP Test ###
add_executable(compare_rsp_test test/manual_tests/compare_rsp_test.cpp)
target_link_libraries(compare_rsp_test rixrdf)

### Unit Tests ###
add_executable(joint_test test/unit_tests/joint_test.cpp)
target_link_libraries(joint_test rixrdf GTest::gtest_main)

add_executable(tree_test test/unit_tests/tree_test.cpp)
target_link_libraries(tree_test rixrdf GTest::gtest_main)

add_executable(fk_solver_test test/unit_tests/fk_solver_test.cpp)
target_link_libraries(fk_solver_test rixrdf GTest::gtest_main)
