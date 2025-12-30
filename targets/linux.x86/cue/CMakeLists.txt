# CUE module libraries

file(GLOB cue_sources 
    "${OPENMRNPATH}/src/cue/*.c"
    "${OPENMRNPATH}/src/cue/*.cpp"
)

if(cue_sources)
    add_library(cue STATIC ${cue_sources})
    target_include_directories(cue PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(cue PUBLIC executor os_main openlcb)
endif()
