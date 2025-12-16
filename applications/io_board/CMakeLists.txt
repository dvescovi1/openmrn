# IO Board Application

file(GLOB IO_BOARD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

openmrn_add_application(io_board
    SOURCES ${IO_BOARD_SOURCES}
    TARGET linux.x86
)
