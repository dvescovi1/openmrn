# Interface module libraries

file(GLOB if_sources 
    "${OPENMRNPATH}/src/if/*.c"
    "${OPENMRNPATH}/src/if/*.cxx"
)

if(if_sources)
    add_library(if STATIC ${if_sources})
    target_include_directories(if PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(if PUBLIC executor os_main openlcb dcc)
endif()
