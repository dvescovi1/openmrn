# Time Server Application

file(GLOB TIME_SERVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

openmrn_add_application(time_server
    SOURCES ${TIME_SERVER_SOURCES}
    BOARD st-stm32h563zi-nucleo
    TARGET freertos.armv8m
)
