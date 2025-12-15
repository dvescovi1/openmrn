cmake_minimum_required(VERSION 3.14)

# Common configuration for all OpenMRN application examples
# Targeting STM32H563 with CMSIS RTOS v2

# Set toolchain before project()
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/arm-none-eabi-h563.cmake CACHE FILEPATH "Toolchain file")
endif()

project(OpenMRN_Applications VERSION 1.0.0 LANGUAGES C CXX ASM)

# Common compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Target definitions
add_compile_definitions(
    STM32H563xx
    USE_HAL_DRIVER
    __CMSIS_RTOS2
)

# Common includes for all applications
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/cmsis_os2
)

# STM32 HAL includes (set STM32CUBEH5_PATH before invoking cmake)
if(DEFINED ENV{STM32CUBEH5_PATH})
    set(STM32CUBEH5_PATH $ENV{STM32CUBEH5_PATH})
endif()

if(STM32CUBEH5_PATH)
    include_directories(
        ${STM32CUBEH5_PATH}/Drivers/STM32H5xx_HAL_Driver/Inc
        ${STM32CUBEH5_PATH}/Drivers/CMSIS/Device/ST/STM32H5xx/Include
        ${STM32CUBEH5_PATH}/Drivers/CMSIS/Include
        ${STM32CUBEH5_PATH}/Drivers/CMSIS/RTOS2/Include
    )
else()
    message(WARNING "STM32CUBEH5_PATH not set. Set environment variable or pass -DSTM32CUBEH5_PATH=<path>")
endif()

# Common linker script location
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/../boards/st-stm32h563-nucleo/STM32H563ZITx_FLASH.ld)

# Application subdirectories
add_subdirectory(async_blink)
add_subdirectory(blink_raw)
add_subdirectory(clinic_app)
add_subdirectory(direct_hub)
add_subdirectory(empty_app)
add_subdirectory(hub)
add_subdirectory(hub_test)
add_subdirectory(io_board)
add_subdirectory(load_test)
add_subdirectory(ping)
add_subdirectory(simple_client)
add_subdirectory(time_client)
add_subdirectory(time_server)

message(STATUS "OpenMRN Applications configured for STM32H563 with CMSIS RTOS v2")
