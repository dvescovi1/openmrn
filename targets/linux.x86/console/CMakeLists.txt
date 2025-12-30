# Console module libraries

file(GLOB console_sources 
    "${OPENMRNPATH}/src/console/*.c"
    "${OPENMRNPATH}/src/console/*.cxx"
)

if(console_sources)
    add_library(console STATIC ${console_sources})
    target_include_directories(console PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(console PUBLIC os_main)
endif()
