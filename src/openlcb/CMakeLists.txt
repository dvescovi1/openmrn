# OpenLCB library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

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
