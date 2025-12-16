# OpenMRN CMake Helper Functions

# ARM Cortex-M toolchain configuration
function(openmrn_configure_arm_cortex target_name)
    cmake_parse_arguments(ARM "" "CPU;FPU;FLOAT_ABI" "" ${ARGN})
    
    if(NOT ARM_CPU)
        set(ARM_CPU "cortex-m4")
    endif()
    
    if(NOT ARM_FPU)
        set(ARM_FPU "fpv4-sp-d16")
    endif()
    
    if(NOT ARM_FLOAT_ABI)
        set(ARM_FLOAT_ABI "hard")
    endif()
    
    target_compile_options(${target_name} PUBLIC
        -mcpu=${ARM_CPU}
        -mthumb
        -mfpu=${ARM_FPU}
        -mfloat-abi=${ARM_FLOAT_ABI}
        -ffunction-sections
        -fdata-sections
        -Wall
    )
    
    target_link_options(${target_name} PUBLIC
        -mcpu=${ARM_CPU}
        -mthumb
        -mfpu=${ARM_FPU}
        -mfloat-abi=${ARM_FLOAT_ABI}
        -Wl,--gc-sections
        -specs=nano.specs
        -specs=nosys.specs
    )
endfunction()

# STM32 board configuration
function(openmrn_configure_stm32 target_name)
    cmake_parse_arguments(STM32 "" "CHIP;BOARD" "SOURCES;INCLUDES" ${ARGN})
    
    if(NOT STM32_CHIP)
        message(FATAL_ERROR "STM32_CHIP must be specified")
    endif()
    
    # Add STM32 HAL includes
    if(DEFINED STM32CUBEPATH)
        target_include_directories(${target_name} PUBLIC
            ${STM32CUBEPATH}/Drivers/STM32${STM32_CHIP}xx_HAL_Driver/Inc
            ${STM32CUBEPATH}/Drivers/CMSIS/Device/ST/STM32${STM32_CHIP}xx/Include
            ${STM32CUBEPATH}/Drivers/CMSIS/Include
        )
    endif()
    
    # Add chip definition
    string(TOUPPER ${STM32_CHIP} STM32_CHIP_UPPER)
    target_compile_definitions(${target_name} PUBLIC STM32${STM32_CHIP_UPPER}xx)
    
    # Add board definition if specified
    if(STM32_BOARD)
        target_compile_definitions(${target_name} PUBLIC ${STM32_BOARD})
    endif()
    
    # Add board-specific sources
    if(STM32_SOURCES)
        target_sources(${target_name} PRIVATE ${STM32_SOURCES})
    endif()
    
    # Add board-specific includes
    if(STM32_INCLUDES)
        target_include_directories(${target_name} PUBLIC ${STM32_INCLUDES})
    endif()
endfunction()

# TI TM4C board configuration
function(openmrn_configure_tm4c target_name)
    cmake_parse_arguments(TM4C "" "CHIP;BOARD" "SOURCES;INCLUDES" ${ARGN})
    
    if(NOT TM4C_CHIP)
        message(FATAL_ERROR "TM4C_CHIP must be specified")
    endif()
    
    # Add TivaWare includes if available
    if(DEFINED TIVAWAREPATH)
        target_include_directories(${target_name} PUBLIC ${TIVAWAREPATH})
    endif()
    
    # Add chip and board definitions
    string(TOUPPER ${TM4C_CHIP} TM4C_CHIP_UPPER)
    target_compile_definitions(${target_name} PUBLIC 
        PART_${TM4C_CHIP_UPPER}
        TARGET_IS_${TM4C_CHIP_UPPER}
    )
    
    if(TM4C_BOARD)
        target_compile_definitions(${target_name} PUBLIC ${TM4C_BOARD})
    endif()
    
    # Add board-specific sources and includes
    if(TM4C_SOURCES)
        target_sources(${target_name} PRIVATE ${TM4C_SOURCES})
    endif()
    
    if(TM4C_INCLUDES)
        target_include_directories(${target_name} PUBLIC ${TM4C_INCLUDES})
    endif()
endfunction()

# Generic board configuration function
function(openmrn_add_board target_name board_path)
    cmake_parse_arguments(BOARD "" "" "SOURCES" ${ARGN})
    
    # Find board files
    set(BOARD_DIR ${OPENMRNPATH}/boards/${board_path})
    
    if(EXISTS ${BOARD_DIR}/HwInit.cxx)
        list(APPEND BOARD_SOURCES ${BOARD_DIR}/HwInit.cxx)
    endif()
    
    if(EXISTS ${BOARD_DIR}/startup.c)
        list(APPEND BOARD_SOURCES ${BOARD_DIR}/startup.c)
    endif()
    
    # Add any additional sources
    if(BOARD_SOURCES)
        list(APPEND BOARD_SOURCES ${BOARD_SOURCES})
    endif()
    
    # Create board library
    if(BOARD_SOURCES)
        add_library(${target_name}-board STATIC ${BOARD_SOURCES})
        target_link_libraries(${target_name} PUBLIC ${target_name}-board)
        target_include_directories(${target_name}-board PUBLIC ${BOARD_DIR})
    endif()
    
    # Add linker script if it exists
    if(EXISTS ${BOARD_DIR}/memory_map.ld)
        target_link_options(${target_name} PUBLIC 
            -T${BOARD_DIR}/memory_map.ld
        )
    endif()
    
    if(EXISTS ${BOARD_DIR}/target.ld)
        target_link_options(${target_name} PUBLIC 
            -T${BOARD_DIR}/target.ld
        )
    endif()
endfunction()

# Helper function to create an OpenMRN application
function(openmrn_add_application app_name)
    cmake_parse_arguments(APP "" "BOARD;TARGET" "SOURCES;LIBS" ${ARGN})
    
    if(NOT APP_SOURCES)
        message(FATAL_ERROR "No sources specified for application ${app_name}")
    endif()
    
    # Create executable
    add_executable(${app_name} ${APP_SOURCES})
    
    # Link with OpenMRN core
    target_link_libraries(${app_name} PRIVATE OpenMRN::core)
    
    # Link with additional libraries
    if(APP_LIBS)
        target_link_libraries(${app_name} PRIVATE ${APP_LIBS})
    endif()
    
    # Configure for board if specified
    if(APP_BOARD)
        openmrn_add_board(${app_name} ${APP_BOARD})
    endif()
    
    # Configure for target if specified
    if(APP_TARGET)
        if(APP_TARGET MATCHES "freertos.armv7m")
            openmrn_configure_arm_cortex(${app_name} CPU cortex-m4)
        elseif(APP_TARGET MATCHES "freertos.armv8m")
            openmrn_configure_arm_cortex(${app_name} CPU cortex-m33)
        elseif(APP_TARGET MATCHES "bare.armv7m")
            openmrn_configure_arm_cortex(${app_name} CPU cortex-m4)
        elseif(APP_TARGET MATCHES "bare.armv8m")
            openmrn_configure_arm_cortex(${app_name} CPU cortex-m33)
        endif()
    endif()
    
    # Set output directory
    set_target_properties(${app_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    
    # Generate binary and hex files for embedded targets
    if(APP_TARGET MATCHES "freertos|bare")
        add_custom_command(TARGET ${app_name} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${app_name}> ${app_name}.bin
            COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${app_name}> ${app_name}.hex
            COMMENT "Generating ${app_name}.bin and ${app_name}.hex"
        )
    endif()
endfunction()
