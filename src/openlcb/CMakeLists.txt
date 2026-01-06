# OpenLCB library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cpp"
)

# Exclude application-specific files that need config.hxx
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/CompileCdiMain.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Stream.cpp")

add_library(openlcb STATIC ${SOURCES})

target_include_directories(openlcb PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(openlcb PUBLIC
    utils
    executor
    os
    dcc
)
