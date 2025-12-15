add_executable(clinic_app
    ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
)

# Find main.cxx and other sources in targets subdirectory
file(GLOB_RECURSE CLINIC_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/targets/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/targets/*.c
)

if(CLINIC_SOURCES)
    target_sources(clinic_app PRIVATE ${CLINIC_SOURCES})
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
    target_sources(clinic_app PRIVATE main.cxx)
endif()

target_link_libraries(clinic_app
    PRIVATE
    openmrn
)

target_link_options(clinic_app PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/clinic_app.map
)

add_custom_command(TARGET clinic_app POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:clinic_app> ${CMAKE_CURRENT_BINARY_DIR}/clinic_app.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:clinic_app> ${CMAKE_CURRENT_BINARY_DIR}/clinic_app.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:clinic_app>
)
