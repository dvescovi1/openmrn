add_executable(hub_test
    main.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

target_link_libraries(hub_test
    PRIVATE
    openmrn
)

target_link_options(hub_test PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/hub_test.map
)

add_custom_command(TARGET hub_test POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:hub_test> ${CMAKE_CURRENT_BINARY_DIR}/hub_test.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:hub_test> ${CMAKE_CURRENT_BINARY_DIR}/hub_test.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:hub_test>
)
