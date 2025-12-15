add_executable(ping
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

# Find main.cxx in targets subdirectory or root
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
    target_sources(ping PRIVATE main.cxx)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/targets/main.cxx)
    target_sources(ping PRIVATE targets/main.cxx)
endif()

target_link_libraries(ping
    PRIVATE
    openmrn
)

target_link_options(ping PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/ping.map
)

add_custom_command(TARGET ping POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:ping> ${CMAKE_CURRENT_BINARY_DIR}/ping.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:ping> ${CMAKE_CURRENT_BINARY_DIR}/ping.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:ping>
)
