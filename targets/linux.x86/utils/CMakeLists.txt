# Utils module libraries

include(${OPENMRNPATH}/cmake/openmrn_library.cmake)

file(GLOB utils_sources 
    "${OPENMRNPATH}/src/utils/*.c"
    "${OPENMRNPATH}/src/utils/*.cxx"
)

if(utils_sources)
    add_library(utils STATIC ${utils_sources})
    target_include_directories(utils PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(utils PUBLIC executor os_main)
endif()
