# Get all test files
file(GLOB_RECURSE TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

# Create test executable
add_executable(test_${PROJECT_NAME} ${TEST_FILES})
target_link_libraries(test_${PROJECT_NAME}  ttt
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(test_${PROJECT_NAME} test_${PROJECT_NAME})

# Override output directory to keep tests in the build directory
set_target_properties(test_${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR})