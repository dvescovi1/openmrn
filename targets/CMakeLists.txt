# Targets CMakeLists.txt
# This directory contains test and configuration targets

if(BUILD_TESTS)
    # Add test subdirectory if it exists
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
        add_subdirectory(test)
    endif()
endif()
