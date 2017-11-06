cmake_minimum_required (VERSION 2.6)
project (NeuroSimulation)

set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

set(SOURCE_FILES src/Neuron.cpp src/Current.cpp src/Network.cpp src/Constants.hpp)


add_executable (NeuroSimulation src/main.cpp ${SOURCE_FILES})


enable_testing()
add_subdirectory(test/googletest)
include_directories(test/googletest/include test/googletest)

add_executable (NeuroSimulation_UnitTest test/main_unittest.cpp ${SOURCE_FILES})

target_link_libraries(NeuroSimulation_UnitTest gtest gtest_main)
add_test(NeuroSimulation_UnitTest NeuroSimulation_UnitTest)


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)