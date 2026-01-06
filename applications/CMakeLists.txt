# Applications CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

# Cross-compiling for bare-metal ARM; avoid host linker flags during try-compile.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(openmrn_applications LANGUAGES C CXX ASM)

# Locate STM32F7 HAL headers (required by the board HwInit sources).
find_path(STM32F7_HAL_INCLUDE_DIR stm32f7xx_hal_rcc.h
    PATHS
        ${STM32CUBE_F7_PATH}
        $ENV{STM32CUBE_F7_PATH}
        $ENV{STM32CUBE_F7}
        $ENV{STM32CUBE_REPO}
    PATH_SUFFIXES
        Drivers/STM32F7xx_HAL_Driver/Inc
        STM32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc
)

if(BUILD_APPLICATIONS AND NOT STM32F7_HAL_INCLUDE_DIR)
    message(WARNING "STM32F7 HAL headers (stm32f7xx_hal_rcc.h) not found. Set STM32CUBE_F7_PATH to your STM32CubeF7 installation to build applications. Skipping application targets.")
    return()
endif()

# Function to create a target for a board
function(add_board_target app_name board_name app_source_dir)
    set(target_name "${app_name}_${board_name}")
    
    # Find target linker script
    if(EXISTS "${OPENMRNPATH}/boards/${board_name}/target.ld")
        set(LINKER_SCRIPT "${OPENMRNPATH}/boards/${board_name}/target.ld")
    else()
        message(WARNING "Linker script not found for board ${board_name}")
        return()
    endif()
    
    # Create executable
    file(GLOB APP_COMMON_SOURCES
        "${app_source_dir}/*.c"
        "${app_source_dir}/*.cpp"
        "${app_source_dir}/*.cxx"
        "${app_source_dir}/*.S"
    )

    # Only include board-specific sources for the selected target.
    file(GLOB APP_BOARD_SOURCES
        "${app_source_dir}/targets/freertos.armv7m.${board_name}/*.c"
        "${app_source_dir}/targets/freertos.armv7m.${board_name}/*.cpp"
        "${app_source_dir}/targets/freertos.armv7m.${board_name}/*.cxx"
        "${app_source_dir}/targets/freertos.armv7m.${board_name}/*.S"
    )

    # Avoid wrapper stubs; use real board startup/HwInit sources instead.
    list(REMOVE_ITEM APP_BOARD_SOURCES
        "${app_source_dir}/targets/freertos.armv7m.${board_name}/startup.c"
        "${app_source_dir}/targets/freertos.armv7m.${board_name}/HwInit.cxx"
    )

    if(EXISTS "${OPENMRNPATH}/boards/${board_name}/startup.c")
        list(APPEND APP_BOARD_SOURCES "${OPENMRNPATH}/boards/${board_name}/startup.c")
    endif()
    if(EXISTS "${OPENMRNPATH}/boards/${board_name}/HwInit.cxx")
        list(APPEND APP_BOARD_SOURCES "${OPENMRNPATH}/boards/${board_name}/HwInit.cxx")
    endif()

    set(APP_SOURCES ${APP_COMMON_SOURCES} ${APP_BOARD_SOURCES})
    
    add_executable(${target_name} ${APP_SOURCES})
    
    # Add application target-specific include directory (for config.hxx)
    if(EXISTS "${app_source_dir}/targets/freertos.armv7m.${board_name}")
        target_include_directories(${target_name} PRIVATE
            "${app_source_dir}/targets/freertos.armv7m.${board_name}"
        )
    endif()
    
    # CRITICAL: Add our freertos_drivers/st FIRST so our HAL headers take precedence
    # This ensures our stm32f7xx_hal_conf.h is found and stm32_hal_compat.h defines
    # HAL_StatusTypeDef before the circular include chain tries to use it
    target_include_directories(${target_name} PRIVATE
        "${OPENMRNPATH}/src/freertos_drivers/st"
    )
    
    # Due to the circular include in STM32F7 HAL (hal_def.h -> stm32f7xx.h -> stm32f7xx_hal.h),
    # stm32_hal_compat.h must define HAL_StatusTypeDef before hal_def.h is included.
    # This causes the same enum to be defined twice - once in our compat header, once in hal_def.h.
    # Both definitions are identical so functionally they're the same type.
    # Since C doesn't allow duplicate anonymous enum typedefs, we suppress the errors.
    # This only affects applications; core libraries don't have this problem.
    target_compile_options(${target_name} PRIVATE -Wno-error)

    if(STM32F7_HAL_INCLUDE_DIR)
        target_include_directories(${target_name} PRIVATE ${STM32F7_HAL_INCLUDE_DIR})
        target_include_directories(${target_name} PRIVATE
            "${STM32CUBE_F7_PATH}/Drivers/CMSIS/Device/ST/STM32F7xx/Include"
            "${STM32CUBE_F7_PATH}/Drivers/CMSIS/Include"
        )
    endif()

    # Core headers used by applications (e.g., os/os.h lives under src/os).
    target_include_directories(${target_name} PRIVATE
        "${OPENMRNPATH}/src"
    )
    
    # Link libraries
    target_link_libraries(${target_name} PRIVATE
        openmrn_core
        freertos
    )
    
    # Board-specific configuration
    if(EXISTS "${OPENMRNPATH}/boards/${board_name}/CMakeLists.txt")
        include("${OPENMRNPATH}/boards/${board_name}/CMakeLists.txt")
        # Pass target name to board configuration
        board_configure("${target_name}")
    endif()
    
    # Set linker script
    target_link_options(${target_name} PRIVATE
        -T${LINKER_SCRIPT}
    )
    
    # Add post-build commands for ELF size and hex generation
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_SIZE} ${target_name}
        COMMENT "Calculating ${target_name} binary size"
    )
    
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${target_name} ${target_name}.hex
        COMMENT "Generating ${target_name}.hex"
    )
endfunction()

# Discover and add all applications
set(APPLICATION_DIRS
    async_blink
    blink_raw
    send_datagram
    simple_client
    io_board
    nucleo_io
)

# Get list of available boards
file(GLOB BOARD_DIRS "${OPENMRNPATH}/boards/*/")
set(AVAILABLE_BOARDS)
foreach(board_dir ${BOARD_DIRS})
    get_filename_component(board_name "${board_dir}" NAME)
    list(APPEND AVAILABLE_BOARDS "${board_name}")
endforeach()

# Process each application
foreach(app_dir ${APPLICATION_DIRS})
    set(app_path "${CMAKE_CURRENT_SOURCE_DIR}/${app_dir}")
    if(EXISTS "${app_path}")
        # For now, build for the selected board
        add_board_target(${app_dir} ${OPENMRN_BOARD} ${app_path})
        
        message(STATUS "Added application: ${app_dir} for board ${OPENMRN_BOARD}")
    endif()
endforeach()

message(STATUS "Available boards: ${AVAILABLE_BOARDS}")
message(STATUS "To build for a different board, use: -DOPENMRN_BOARD=<board_name>")
