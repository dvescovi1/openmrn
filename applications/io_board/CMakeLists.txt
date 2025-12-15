add_executable(io_board
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

# Find main.cxx in targets subdirectory or root
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
    target_sources(io_board PRIVATE main.cxx)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/targets/main.cxx)
    target_sources(io_board PRIVATE targets/main.cxx)
endif()

target_link_libraries(io_board
    PRIVATE
    openmrn
)

target_link_options(io_board PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/io_board.map
)

add_custom_command(TARGET io_board POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:io_board> ${CMAKE_CURRENT_BINARY_DIR}/io_board.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:io_board> ${CMAKE_CURRENT_BINARY_DIR}/io_board.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:io_board>
)
