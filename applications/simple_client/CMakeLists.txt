add_executable(simple_client
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(simple_client
    PRIVATE
    openmrn
)

target_link_options(simple_client PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/simple_client.map
)

add_custom_command(TARGET simple_client POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:simple_client> ${CMAKE_CURRENT_BINARY_DIR}/simple_client.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:simple_client> ${CMAKE_CURRENT_BINARY_DIR}/simple_client.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:simple_client>
)
