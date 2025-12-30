# DCC module libraries

include(${OPENMRNPATH}/cmake/openmrn_library.cmake)

file(GLOB dcc_sources 
    "${OPENMRNPATH}/src/dcc/*.c"
    "${OPENMRNPATH}/src/dcc/*.cpp"
)

if(dcc_sources)
    add_library(dcc STATIC ${dcc_sources})
    target_include_directories(dcc PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(dcc PUBLIC executor os_main)
endif()
