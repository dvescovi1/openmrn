# Configuration for bare-metal ARM v7-M target (no OS)

# ARM Cortex-M3 specific flags
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft ${CMAKE_CXX_FLAGS}")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft ${CMAKE_ASM_FLAGS}")

# Bare metal compile options
add_compile_options(
    -mfix-cortex-m3-ldrd
    -fno-builtin
    -fno-stack-protector
)

include_directories(${OPENMRNPATH}/include)

message(STATUS "Bare Metal Cortex-M3 Target Configuration")
