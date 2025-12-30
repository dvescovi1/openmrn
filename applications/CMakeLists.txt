# Applications subdirectory CMakeLists.txt

# Define all available applications
set(APPLICATIONS
    async_blink
    blink_raw
    bootloader
    bootloader_client
    can_eth
    reflash_bootloader
    clinic_app
    hub
    io_board
    js_hub
    js_client
    js_cdi_server
    memconfig_utils
    send_datagram
    simple_client
    tractionproxy
    train
    tcp_blink_client
    usb_can
    time_client
    time_server
)

# Add subdirectories for each application that has a CMakeLists.txt
foreach(app ${APPLICATIONS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${app}/CMakeLists.txt")
        add_subdirectory(${app})
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${app}")
        message(STATUS "Skipping application ${app} - CMakeLists.txt not yet migrated")
    endif()
endforeach()

# Convenience target
add_custom_target(all-applications
    COMMENT "Build all available applications"
)
