cmake_minimum_required(VERSION 3.20)

project(OpenMRN_CMSIS_OS2_Example)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define CMSIS-RTOS2 platform
add_definitions(-D__CMSIS_RTOS2)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/inc/cmsis_os2
    ${CMAKE_SOURCE_DIR}/src
)

# Example executable
add_executable(cmsis_os2_example
    SimpleCanClient.cxx
)

# Link with OpenMRN (header-only, but needs compilation of selected sources)
target_link_libraries(cmsis_os2_example
    PRIVATE
    OpenMRN::openmrn
)

# Compiler flags for ARM Cortex-M
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-m4 -mthumb -fPIC")
