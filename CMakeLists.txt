cmake_minimum_required (VERSION 2.6)
project (NeuroSimulation)

set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(src/googletest)
include_directories(src/googletest/include src/googletest)

add_executable (NeuroSimulation src/main.cpp src/Neuron.cpp src/Current.cpp src/Network.cpp src/Constants.hpp)
add_executable (NeuroSimulation_UnitTest src/main_unittest.cpp src/Neuron.cpp src/Current.cpp src/Network.cpp src/Constants.hpp)

target_link_libraries(NeuroSimulation_UnitTest gtest gtest_main)
add_test(NeuroSimulation_UnitTest NeuroSimulation_UnitTest)
