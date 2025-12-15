add_executable(empty_app
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(empty_app
    PRIVATE
    openmrn
)

target_link_options(empty_app PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/empty_app.map
)

add_custom_command(TARGET empty_app POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:empty_app> ${CMAKE_CURRENT_BINARY_DIR}/empty_app.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:empty_app> ${CMAKE_CURRENT_BINARY_DIR}/empty_app.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:empty_app>
)
