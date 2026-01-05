# FreeRTOS drivers and STM32F7 HAL glue

# Build HAL from STM32CubeF7 if available (needed for STM32F767 targets).
if(NOT STM32CUBE_F7_PATH)
    message(FATAL_ERROR "STM32CUBE_F7_PATH is required to build freertos_drivers for STM32F7 boards.")
endif()

set(HAL_DRIVER_DIR "${STM32CUBE_F7_PATH}/Drivers/STM32F7xx_HAL_Driver")
set(HAL_DRIVER_SRC_DIR "${HAL_DRIVER_DIR}/Src")
set(HAL_CMSIS_DEVICE_DIR "${STM32CUBE_F7_PATH}/Drivers/CMSIS/Device/ST/STM32F7xx")

set(HAL_SOURCES
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_cortex.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_rcc.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_rcc_ex.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_pwr.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_pwr_ex.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_gpio.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_dma.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_dma_ex.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_tim.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_tim_ex.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_uart.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_uart_ex.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_can.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_flash.c
    ${HAL_DRIVER_SRC_DIR}/stm32f7xx_hal_flash_ex.c
)

set(HAL_SYSTEM_FILE ${HAL_CMSIS_DEVICE_DIR}/Source/Templates/system_stm32f7xx.c)

add_library(stm32f7_hal STATIC ${HAL_SOURCES} ${HAL_SYSTEM_FILE})
target_include_directories(stm32f7_hal PUBLIC
    ${OPENMRNPATH}/src/freertos_drivers/st
    ${HAL_DRIVER_DIR}/Inc
    ${HAL_CMSIS_DEVICE_DIR}/Include
    ${STM32CUBE_F7_PATH}/Drivers/CMSIS/Include
)
target_compile_definitions(stm32f7_hal PUBLIC USE_HAL_DRIVER STM32F767xx)

# Suppress return-type warnings in HAL library for C code
target_compile_options(stm32f7_hal PRIVATE $<$<COMPILE_LANGUAGE:C>:-Wno-error=return-type>)

# FreeRTOS driver layer
file(GLOB FREERTOS_DRIVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/st/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/st/*.cxx
)

list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX "libatomic\\.c$")
# TinyUSB glue and FDCAN driver are not wired for STM32F767 builds yet.
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32UsbCdc\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32MCan\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32UsbCdc\\.hxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32MCan\\.hxx$")
# Exclude POSIX-based drivers that don't work in bare-metal
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Device\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/DeviceFile\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/FileSystem\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Pipe\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Serial\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Socket\\.cxx$")
# The generic Can.cxx also uses POSIX
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/common/Can\\.cxx$")
# MCAN driver includes POSIX device abstraction APIs
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/common/MCAN\\.cxx$")
# Exclude other Devtab-dependent files (all I/O device abstraction layer files)
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/SPI\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/SPIFlash\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Select\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Null\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Node\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/I2C\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/FileioWeak\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Fileio\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/EEPROM\\.cxx$")
# Exclude CAN/device drivers with POSIX dependencies (fcntl, unistd, stropts)
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/TCAN4550Can\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/MCP2515Can\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/WifiDefs\\.cxx$")
# Exclude EEPROM emulation drivers with device abstraction dependencies
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/EEPROMEmulation\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/EEPROMEmulation_weak\\.cxx$")
# Exclude other device interface files that aren't needed for minimal build
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/CpuLoad\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/PCA9685PWM\\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/SN74HC595GPO\\.cxx$")
# Exclude ST firmware-specific drivers from minimal build
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32EEPROMEmulation\\.cxx$")
# Stm32Can.cxx includes generic Can.hxx which depends on Devtab for device abstraction
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32Can\\.cxx$")
# Exclude STM32 peripheral drivers that pull in Devtab/POSIX abstractions
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32I2C\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32SPI\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32RailcomSender\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32Uart\.cxx$")
list(FILTER FREERTOS_DRIVER_SOURCES EXCLUDE REGEX ".*/Stm32Flash\.cxx$")

add_library(freertos_drivers STATIC ${FREERTOS_DRIVER_SOURCES})
target_include_directories(freertos_drivers PUBLIC
    ${OPENMRNPATH}/src/freertos_drivers/st
    ${OPENMRNPATH}/src/freertos_drivers/common
    ${OPENMRNPATH}/src
)

# STM32 HAL headers have circular include issues in bare-metal context
# Disable treating errors as fatal specifically for C++ in this target
target_compile_options(freertos_drivers BEFORE PRIVATE 
    $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<BOOL:-Werror>>:-Wno-error>
)

target_link_libraries(freertos_drivers PUBLIC freertos stm32f7_hal)
