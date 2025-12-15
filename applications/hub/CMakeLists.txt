add_executable(hub
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(hub
    PRIVATE
    openmrn
)

target_link_options(hub PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/hub.map
)

add_custom_command(TARGET hub POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:hub> ${CMAKE_CURRENT_BINARY_DIR}/hub.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:hub> ${CMAKE_CURRENT_BINARY_DIR}/hub.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:hub>
)
