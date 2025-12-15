add_executable(blink_raw
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(blink_raw
    PRIVATE
    openmrn
)

target_link_options(blink_raw PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/blink_raw.map
)

# Generate binary and hex files
add_custom_command(TARGET blink_raw POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:blink_raw> ${CMAKE_CURRENT_BINARY_DIR}/blink_raw.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:blink_raw> ${CMAKE_CURRENT_BINARY_DIR}/blink_raw.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:blink_raw>
)
