# Source libraries CMakeLists.txt

# Add src directory to include path for all libraries so that
# #include "module/header.hxx" patterns work correctly
include_directories(${OPENMRNPATH}/src)

# Define core libraries
set(CORELIBS
    console
    utils
    executor
    os
    dcc
    openlcb
    withrottle
    ble
    traction_modem
)

# Add each library as a subdirectory
foreach(lib ${CORELIBS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${lib}/CMakeLists.txt")
        add_subdirectory(${lib})
    endif()
endforeach()

# Create an interface library that links all core libraries
add_library(openmrn_core INTERFACE)
foreach(lib ${CORELIBS})
    target_link_libraries(openmrn_core INTERFACE ${lib})
endforeach()
