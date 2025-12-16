# Train Application

file(GLOB TRAIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

openmrn_add_application(train
    SOURCES ${TRAIN_SOURCES}
    TARGET linux.x86
)
