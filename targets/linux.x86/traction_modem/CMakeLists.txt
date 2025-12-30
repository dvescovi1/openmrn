# Traction modem module libraries

file(GLOB traction_modem_sources 
    "${OPENMRNPATH}/src/traction_modem/*.c"
    "${OPENMRNPATH}/src/traction_modem/*.cxx"
)

if(traction_modem_sources)
    add_library(traction_modem STATIC ${traction_modem_sources})
    target_include_directories(traction_modem PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(traction_modem PUBLIC executor os_main openlcb)
endif()
