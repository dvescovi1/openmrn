# OpenMRN Tests

# Find or download GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()
include(GoogleTest)

# Function to add a test from .cxxtest file
function(add_openmrn_test TEST_FILE)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    get_filename_component(TEST_DIR ${TEST_FILE} DIRECTORY)
    
    # Make test name unique by including directory path
    string(REPLACE "/" "_" TEST_TARGET "${TEST_DIR}_${TEST_NAME}")
    string(REPLACE "\\" "_" TEST_TARGET "${TEST_TARGET}")
    
    # Create test executable
    add_executable(${TEST_TARGET}_test ${CMAKE_CURRENT_SOURCE_DIR}/../src/${TEST_FILE})
    
    # Link with libraries
    target_link_libraries(${TEST_TARGET}_test
        PRIVATE
            OpenMRN::openmrn
            gtest_main
            gmock
    )
    
    # Add include directories
    target_include_directories(${TEST_TARGET}_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    # Add to tests
    gtest_discover_tests(${TEST_TARGET}_test)
endfunction()

# Collect all .cxxtest files
file(GLOB_RECURSE TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cxxtest
)

# Add each test
foreach(TEST_FILE ${TEST_FILES})
    # Make path relative to src directory
    file(RELATIVE_PATH REL_TEST_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../src ${TEST_FILE})
    add_openmrn_test(${REL_TEST_FILE})
endforeach()
