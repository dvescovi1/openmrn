# Simple Client Application

file(GLOB SIMPLE_CLIENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

openmrn_add_application(simple_client
    SOURCES ${SIMPLE_CLIENT_SOURCES}
    TARGET linux.x86
)
