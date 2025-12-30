# Withrottle module libraries

file(GLOB withrottle_sources 
    "${OPENMRNPATH}/src/withrottle/*.c"
    "${OPENMRNPATH}/src/withrottle/*.cpp"
)

if(withrottle_sources)
    add_library(withrottle STATIC ${withrottle_sources})
    target_include_directories(withrottle PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
    target_link_libraries(withrottle PUBLIC executor os_main openlcb dcc if)
endif()
