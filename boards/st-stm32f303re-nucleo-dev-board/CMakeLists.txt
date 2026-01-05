# STM32F3 Dev Board Configuration

function(board_configure target_name)
    set(BOARD_CPU_ARCH "cortex-m4")
    set(BOARD_FLOAT_ABI "hard")
    set(BOARD_FPU "fpv4-sp-d16")

    # STM32F3 specific flags
    target_compile_definitions(${target_name} PRIVATE
        STM32F303xE
    )

    # Board include directories
    target_include_directories(${target_name} PRIVATE
        ${OPENMRNPATH}/boards/st-stm32f303re-nucleo-dev-board
    )

    # Add board-specific source files
    file(GLOB BOARD_SOURCES
        "${OPENMRNPATH}/boards/st-stm32f303re-nucleo-dev-board/*.c"
        "${OPENMRNPATH}/boards/st-stm32f303re-nucleo-dev-board/*.cxx"
        "${OPENMRNPATH}/boards/st-stm32f303re-nucleo-dev-board/*.S"
    )

    target_sources(${target_name} PRIVATE ${BOARD_SOURCES})

    message(STATUS "Board Configuration: STM32F303RE Nucleo Dev Board (Cortex-M4) - ${target_name}")
endfunction()

