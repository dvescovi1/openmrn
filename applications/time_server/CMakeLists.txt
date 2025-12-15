add_executable(time_server
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(time_server
    PRIVATE
    openmrn
)

target_link_options(time_server PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/time_server.map
)

add_custom_command(TARGET time_server POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:time_server> ${CMAKE_CURRENT_BINARY_DIR}/time_server.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:time_server> ${CMAKE_CURRENT_BINARY_DIR}/time_server.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:time_server>
)
