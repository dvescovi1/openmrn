add_executable(async_blink
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(async_blink
    PRIVATE
    openmrn
)

target_link_options(async_blink PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/async_blink.map
)

add_custom_command(TARGET async_blink POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:async_blink> ${CMAKE_CURRENT_BINARY_DIR}/async_blink.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:async_blink> ${CMAKE_CURRENT_BINARY_DIR}/async_blink.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:async_blink>
)
