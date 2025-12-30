# BLE module libraries

file(GLOB ble_sources 
    "${OPENMRNPATH}/src/ble/*.c"
    "${OPENMRNPATH}/src/ble/*.cxx"
)

if(ble_sources)
    add_library(ble STATIC ${ble_sources})
    target_include_directories(ble PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(ble PUBLIC executor os_main openlcb)
endif()
