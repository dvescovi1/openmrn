# CMake Toolchain file for ARM Cortex-M targets

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-as)

# Disable linking shared libraries
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Common compiler flags for ARM Cortex-M
set(ARM_CORTEX_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS_INIT "${ARM_CORTEX_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${ARM_CORTEX_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${ARM_CORTEX_FLAGS}")

# Add other target-specific options
add_compile_options(
    -Wall
    -Wextra
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -fno-rtti
)

# Link options
add_link_options(
    -Wl,--gc-sections
    -Wl,-Map=output.map
)
