cmake_minimum_required(VERSION 3.17)
project(taking_the_temperature)

set(CMAKE_CXX_STANDARD 17)
add_definitions("-Wall")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib")

option(ENABLE_DOC "Generates the documentation target" OFF)
option(ENABLE_COVERAGE "Generates the coverage build" OFF)
option(ENABLE_TESTING "Turns on testing" OFF)

if (ENABLE_DOC)
    add_subdirectory(docs)
endif ()


# Set default install prefix
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

IF (WIN32)
    set(Boost_INCLUDE_DIR C:/Boost_gcc/include)
    set(Boost_LIBRARY_DIR C:/Boost_gcc/lib)
    set(Boost_ROOT C:/Boost_gcc)
ENDIF ()

set(Boost_DEBUG 1)
find_package(Boost
        REQUIRED COMPONENTS
        chrono
        date_time
        iostreams
        system
        filesystem
        unit_test_framework
        regex)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(libs)

if(ENABLE_COVERAGE)
    set(CMAKE_BUILD_TYPE "Coverage")
    include(CodeCoverage)
    add_definitions("-Wall -pedantic -g -O0 -fprofile-arcs -ftest-coverage")
    setup_target_for_coverage(${PROJECT_NAME}_coverage ${PROJECT_NAME}_test
            coverage)
endif()

if (ENABLE_TESTING OR ENABLE_COVERAGE)
    enable_testing()                                                                          #
    add_subdirectory(tests)
endif ()

if (ENABLE_DOC)
    add_subdirectory(docs)
endif ()

add_subdirectory(src)
