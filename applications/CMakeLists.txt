# Applications CMakeLists.txt

# Function to create a target for a board
function(add_board_target app_name board_name app_source_dir)
    set(target_name "${app_name}_${board_name}")
    
    # Find target linker script
    if(EXISTS "${OPENMRNPATH}/boards/${board_name}/target.ld")
        set(LINKER_SCRIPT "${OPENMRNPATH}/boards/${board_name}/target.ld")
    else()
        message(WARNING "Linker script not found for board ${board_name}")
        return()
    endif()
    
    # Create executable
    file(GLOB_RECURSE APP_SOURCES
        "${app_source_dir}/*.c"
        "${app_source_dir}/*.cpp"
        "${app_source_dir}/*.cxx"
        "${app_source_dir}/*.S"
    )
    
    add_executable(${target_name} ${APP_SOURCES})
    
    # Link libraries
    target_link_libraries(${target_name} PRIVATE
        openmrn_core
        freertos
    )
    
    # Board-specific configuration
    if(EXISTS "${OPENMRNPATH}/boards/${board_name}/CMakeLists.txt")
        include("${OPENMRNPATH}/boards/${board_name}/CMakeLists.txt")
        # Pass target name to board configuration
        board_configure("${target_name}")
    endif()
    
    # Set linker script
    target_link_options(${target_name} PRIVATE
        -T${LINKER_SCRIPT}
    )
    
    # Add post-build commands for ELF size and hex generation
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_SIZE} ${target_name}
        COMMENT "Calculating ${target_name} binary size"
    )
    
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${target_name} ${target_name}.hex
        COMMENT "Generating ${target_name}.hex"
    )
endfunction()

# Discover and add all applications
set(APPLICATION_DIRS
    async_blink
    blink_raw
    send_datagram
    simple_client
    io_board
    nucleo_io
)

# Get list of available boards
file(GLOB BOARD_DIRS "${OPENMRNPATH}/boards/*/")
set(AVAILABLE_BOARDS)
foreach(board_dir ${BOARD_DIRS})
    get_filename_component(board_name "${board_dir}" NAME)
    list(APPEND AVAILABLE_BOARDS "${board_name}")
endforeach()

# Process each application
foreach(app_dir ${APPLICATION_DIRS})
    set(app_path "${CMAKE_CURRENT_SOURCE_DIR}/${app_dir}")
    if(EXISTS "${app_path}")
        # For now, build for the selected board
        add_board_target(${app_dir} ${OPENMRN_BOARD} ${app_path})
        
        message(STATUS "Added application: ${app_dir} for board ${OPENMRN_BOARD}")
    endif()
endforeach()

message(STATUS "Available boards: ${AVAILABLE_BOARDS}")
message(STATUS "To build for a different board, use: -DOPENMRN_BOARD=<board_name>")
