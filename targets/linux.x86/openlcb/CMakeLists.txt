# OpenLCB module libraries

include(${OPENMRNPATH}/cmake/openmrn_library.cmake)

file(GLOB openlcb_sources 
    "${OPENMRNPATH}/src/openlcb/*.c"
    "${OPENMRNPATH}/src/openlcb/*.cxx"
)

if(openlcb_sources)
    add_library(openlcb STATIC ${openlcb_sources})
    target_include_directories(openlcb PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(openlcb PUBLIC executor utils os_main dcc)
endif()
