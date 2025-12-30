# OS module libraries

include(${OPENMRNPATH}/cmake/openmrn_library.cmake)

# Create libraries from OS source files
file(GLOB os_modules "${OPENMRNPATH}/src/os/*")
foreach(module ${os_modules})
    if(IS_DIRECTORY "${module}")
        get_filename_component(module_name "${module}" NAME)
        if(NOT module_name MATCHES "^CMake" AND NOT module_name MATCHES "^\\.")
            file(GLOB sources "${module}/*.c" "${module}/*.cpp" "${module}/*.S")
            if(sources)
                set(target_name "os_${module_name}")
                add_library(${target_name} STATIC ${sources})
                target_include_directories(${target_name} PUBLIC
                    ${OPENMRNPATH}/include
                    ${OPENMRNPATH}/src
                )
            endif()
        endif()
    endif()
endforeach()

# Create main OS library from direct source files in os/
file(GLOB os_sources "${OPENMRNPATH}/src/os/*.c" "${OPENMRNPATH}/src/os/*.cpp")
if(os_sources)
    add_library(os_main STATIC ${os_sources})
    target_include_directories(os_main PUBLIC
        ${OPENMRNPATH}/include
        ${OPENMRNPATH}/src
    )
endif()
