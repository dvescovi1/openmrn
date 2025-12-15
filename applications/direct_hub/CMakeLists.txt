cmake_minimum_required(VERSION 3.14)

# Define the executable
add_executable(direct_hub
    main.cxx
    ${CMAKE_SOURCE_DIR}/boards/st-stm32h563-nucleo/startup.c
)

# Link with OpenMRN library
target_link_libraries(direct_hub PRIVATE openmrn)

# Set linker script and map file
set_target_properties(direct_hub PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/boards/st-stm32h563-nucleo/STM32H563ZITx_FLASH.ld -Wl,-Map=direct_hub.map,--cref,--no-warn-mismatch"
    SUFFIX ".elf"
)

# Generate hex and bin files
add_custom_command(TARGET direct_hub POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:direct_hub> direct_hub.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:direct_hub> direct_hub.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:direct_hub>
    COMMENT "Generating HEX and BIN files for direct_hub"
)
