# Executor module libraries

include(${OPENMRNPATH}/cmake/openmrn_library.cmake)

file(GLOB executor_sources 
    "${OPENMRNPATH}/src/executor/*.c"
    "${OPENMRNPATH}/src/executor/*.cpp"
)

if(executor_sources)
    add_library(executor STATIC ${executor_sources})
    target_include_directories(executor PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
endif()
