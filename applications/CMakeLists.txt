# OpenMRN Applications

# Macro to add an application
macro(add_openmrn_application APP_NAME APP_DIR)
    # Check if the main.cxx exists
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${APP_DIR}/main.cxx)
        # Collect source files
        file(GLOB APP_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/${APP_DIR}/*.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/${APP_DIR}/*.c
        )
        
        # Create executable
        add_executable(${APP_NAME} ${APP_SOURCES})
        
        # Link with openmrn library
        target_link_libraries(${APP_NAME} PRIVATE OpenMRN::openmrn)
        
        # Set include directories
        target_include_directories(${APP_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/${APP_DIR}
        )
        
        # Platform-specific libraries
        if(LINUX)
            target_link_libraries(${APP_NAME} PRIVATE Threads::Threads)
            if(AVAHI_FOUND)
                target_link_libraries(${APP_NAME} PRIVATE ${AVAHI_LIBRARIES})
            endif()
        endif()
        
        # Install
        install(TARGETS ${APP_NAME}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
    endif()
endmacro()

# Add applications
add_openmrn_application(hub hub)
add_openmrn_application(memconfig_utils memconfig_utils)
add_openmrn_application(bootloader_client bootloader_client)
add_openmrn_application(send_datagram send_datagram)
add_openmrn_application(async_blink async_blink)
add_openmrn_application(can_eth can_eth)
add_openmrn_application(hub_test hub_test)
add_openmrn_application(io_board io_board)
add_openmrn_application(ping ping)
add_openmrn_application(simple_client simple_client)
add_openmrn_application(tcp_blink_client tcp_blink_client)
add_openmrn_application(time_client time_client)
add_openmrn_application(time_server time_server)
add_openmrn_application(tractionproxy tractionproxy)
add_openmrn_application(train train)
