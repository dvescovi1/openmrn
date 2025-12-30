# Targets subdirectory CMakeLists.txt
# This builds the target-specific libraries and executables

# Define all available targets
set(TARGETS
    linux.x86
    linux.llvm
    linux.armv7a
    freertos.armv7m
    freertos.armv6m
    mach.x86_64
    bare.armv7m
    js.emscripten
    mingw.x86
    nonos.xtensa-lx106.esp8266
)

# Add subdirectories for each target
foreach(target ${TARGETS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${target}/CMakeLists.txt")
        add_subdirectory(${target})
        message(STATUS "Added target: ${target}")
    else()
        message(STATUS "Skipping target ${target} - CMakeLists.txt not yet created")
    endif()
endforeach()

# Convenience targets
add_custom_target(all-targets
    COMMENT "Build all available targets"
)

# Test target
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()
