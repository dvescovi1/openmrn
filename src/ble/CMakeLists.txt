# BLE library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

add_library(ble STATIC ${SOURCES})

target_include_directories(ble PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(ble PUBLIC
    utils
    executor
    os
)
