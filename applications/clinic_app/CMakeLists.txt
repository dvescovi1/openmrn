# Clinic App Application

file(GLOB CLINIC_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

openmrn_add_application(clinic_app
    SOURCES ${CLINIC_APP_SOURCES}
    BOARD st-stm32h563zi-nucleo
    TARGET freertos.armv8m
)
