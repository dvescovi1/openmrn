# STM32F7 Nucleo Board Configuration

function(board_configure target_name)
    set(BOARD_CPU_ARCH "cortex-m7")
    set(BOARD_FLOAT_ABI "hard")
    set(BOARD_FPU "fpv5-d16")

    # STM32F7 specific flags
    target_compile_definitions(${target_name} PRIVATE
        STM32F767xx
    )

    # Board include directories
    target_include_directories(${target_name} PRIVATE
        ${OPENMRNPATH}/boards/st-stm32f767zi-nucleo
    )

    # Add board-specific source files
    file(GLOB BOARD_SOURCES
        "${OPENMRNPATH}/boards/st-stm32f767zi-nucleo/*.c"
        "${OPENMRNPATH}/boards/st-stm32f767zi-nucleo/*.cxx"
        "${OPENMRNPATH}/boards/st-stm32f767zi-nucleo/*.S"
    )

    target_sources(${target_name} PRIVATE ${BOARD_SOURCES})

    message(STATUS "Board Configuration: STM32F767ZI Nucleo (Cortex-M7) - ${target_name}")
endfunction()

