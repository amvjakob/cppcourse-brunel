cmake_minimum_required (VERSION 2.6)
project (NeuroSimulation)

set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(googletest)
include_directories(googletest/include googletest)

add_executable (NeuroSimulation main.cpp Neuron.cpp Current.cpp Network.cpp Constants.hpp)
add_executable (NeuroSimulation_UnitTest main_unittest.cpp Neuron.cpp Current.cpp Network.cpp Constants.hpp)

target_link_libraries(NeuroSimulation_UnitTest gtest gtest_main)
add_test(NeuroSimulation_UnitTest NeuroSimulation_UnitTest)
