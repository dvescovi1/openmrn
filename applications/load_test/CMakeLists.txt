cmake_minimum_required(VERSION 3.14)

# Define the executable
add_executable(load_test
    main.cxx
    ${CMAKE_SOURCE_DIR}/boards/st-stm32h563-nucleo/startup.c
)

# Link with OpenMRN library
target_link_libraries(load_test PRIVATE openmrn)

# Set linker script and map file
set_target_properties(load_test PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/boards/st-stm32h563-nucleo/STM32H563ZITx_FLASH.ld -Wl,-Map=load_test.map,--cref,--no-warn-mismatch"
    SUFFIX ".elf"
)

# Generate hex and bin files
add_custom_command(TARGET load_test POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:load_test> load_test.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:load_test> load_test.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:load_test>
    COMMENT "Generating HEX and BIN files for load_test"
)
