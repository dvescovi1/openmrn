# Traction Modem library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

add_library(traction_modem STATIC ${SOURCES})

target_include_directories(traction_modem PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(traction_modem PUBLIC
    utils
    executor
    os
    dcc
)
