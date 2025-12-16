# Time Client Application

file(GLOB TIME_CLIENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

openmrn_add_application(time_client
    SOURCES ${TIME_CLIENT_SOURCES}
    BOARD st-stm32h563zi-nucleo
    TARGET freertos.armv8m
)
