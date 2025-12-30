# Core libraries for linux.x86 target

# Helper function to create a library from a source subdirectory
function(create_openmrn_lib lib_path parent_lib)
    get_filename_component(lib_name "${lib_path}" NAME)
    set(lib_target "${parent_lib}_${lib_name}")
    
    file(GLOB_RECURSE sources
        "${OPENMRNPATH}/src/${lib_path}/*.c"
        "${OPENMRNPATH}/src/${lib_path}/*.cxx"
        "${OPENMRNPATH}/src/${lib_path}/*.cpp"
        "${OPENMRNPATH}/src/${lib_path}/*.S"
    )
    
    if(sources)
        add_library(${lib_target} STATIC ${sources})
        target_include_directories(${lib_target} PUBLIC
            ${OPENMRNPATH}/include
            ${OPENMRNPATH}/src
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
        set_target_properties(${lib_target} PROPERTIES
            COMPILE_FLAGS "-m32"
            POSITION_INDEPENDENT_CODE ON
        )
    endif()
endfunction()

# Add all core subdirectories
add_subdirectory(os)
add_subdirectory(executor)
add_subdirectory(dcc)
add_subdirectory(openlcb)
add_subdirectory(utils)
add_subdirectory(withrottle)
add_subdirectory(if)
add_subdirectory(console)
add_subdirectory(ble)
add_subdirectory(cue)
add_subdirectory(traction_modem)
