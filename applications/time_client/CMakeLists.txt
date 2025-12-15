add_executable(time_client
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(time_client
    PRIVATE
    openmrn
)

target_link_options(time_client PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/time_client.map
)

add_custom_command(TARGET time_client POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:time_client> ${CMAKE_CURRENT_BINARY_DIR}/time_client.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:time_client> ${CMAKE_CURRENT_BINARY_DIR}/time_client.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:time_client>
)
