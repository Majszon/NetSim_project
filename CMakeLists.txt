cmake_minimum_required(VERSION 3.21)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(NetSim main.cpp)

set(PROJECT_ID NetSim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic -errors)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
)

add_executable(tsp ${SOURCE_FILES} main.cpp)

set(SOURCES_FILES_TESTS
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp
)

add_executable(NetSim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(NetSim_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
)

target_link_libraries(NetSim_test gmock)
