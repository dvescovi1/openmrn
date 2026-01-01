# Default CMake configuration for OpenMRN
# This file sets up common compiler flags and options

# Common compiler flags
if(NOT MSVC)
    # GCC/Clang flags
    add_compile_options(-Wall -Wextra -Wno-unused-parameter)
    add_compile_options(-fno-strict-aliasing)
    add_compile_options(-Wno-deprecated-declarations)
endif()

# Feature options
option(BUILD_TESTING "Enable testing" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_LTO "Enable Link Time Optimization" OFF)

if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported)
    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

# Default include directories that most targets need
include_directories(${OPENMRNPATH}/include)
include_directories(${OPENMRNPATH}/src)
