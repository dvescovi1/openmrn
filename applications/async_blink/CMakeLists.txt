# Async Blink Application

openmrn_add_application(async_blink
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx
    BOARD st-stm32h563zi-nucleo
    TARGET freertos.armv8m
)
