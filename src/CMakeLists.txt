project(openmrn)

if(NOT DEFINED OPENMRNPATH)
set(OPENMRNPATH $ENV{OPENMRNPATH})
endif()

# Default option values
option(FREERTOS "freertos" OFF)
option(USE_DEVICE_FILE_SYSTEM "use device file system" OFF)
option(TARGET_BARE_ARMV7M     "bare armv7m"            OFF)
option(TARGET_ARMV7M          "bare armv7m"            OFF)
option(TARGET_BARE_ARMV6M     "bare armv6m"            OFF)
option(TARGET_ARMV6M          "bare armv6m"            OFF)
option(TARGET_SPIFFS          "spiffs"                 OFF)
option(TARGET_TM4C129         "tm4c129"                OFF)

# Collect sources using wildcards
file(GLOB_RECURSE SOURCES
    "${OPENMRNPATH}/src/ble/*.cpp"
    "${OPENMRNPATH}/src/ble/*.c"
    "${OPENMRNPATH}/src/console/*.cpp"
    "${OPENMRNPATH}/src/console/*.c"
    "${OPENMRNPATH}/src/dcc/*.cpp"
    "${OPENMRNPATH}/src/dcc/*.c"
    "${OPENMRNPATH}/src/executor/*.cpp"
    "${OPENMRNPATH}/src/executor/*.c"
    "${OPENMRNPATH}/src/openlcb/*.cpp"
    "${OPENMRNPATH}/src/openlcb/*.c"
    "${OPENMRNPATH}/src/os/*.cpp"
    "${OPENMRNPATH}/src/os/*.c"
    "${OPENMRNPATH}/src/traction_modem/*.cpp"
    "${OPENMRNPATH}/src/traction_modem/*.c"
    "${OPENMRNPATH}/src/utils/*.cpp"
    "${OPENMRNPATH}/src/utils/*.c"
)

if(${FREERTOS})         
file(GLOB FREERTOS_SOURCES
    "${FREERTOSPATH}/Source/croutine.c"
    "${FREERTOSPATH}/Source/event_groups.c"
    "${FREERTOSPATH}/Source/list.c"
    "${FREERTOSPATH}/Source/queue.c"
    "${FREERTOSPATH}/Source/tasks.c"
    "${FREERTOSPATH}/Source/timers.c"
    "${FREERTOSPATH}/Source/portable/GCC/ARM_CM3/port.c"
    "${FREERTOSPATH}/Source/portable/MemMang/heap_3.c"
)
set(SOURCES ${SOURCES} ${FREERTOS_SOURCES})
endif() # FREERTOS

if(${USE_DEVICE_FILE_SYSTEM})
file(GLOB_RECURSE DEVICE_FS_SOURCES
    "${OPENMRNPATH}/src/freertos_drivers/common/*.cpp"
    "${OPENMRNPATH}/src/freertos_drivers/common/*.c"
)
set(SOURCES ${SOURCES} ${DEVICE_FS_SOURCES})
endif() # USE_DEVICE_FILE_SYSTEM


if (${GTEST})
# Collect all test files using wildcards
file(GLOB_RECURSE TESTS
    "${OPENMRNPATH}/src/**/*.cpptest"
)
set(TESTS ${TESTS} PARENT_SCOPE)
endif() # ${GTEST}

# Common openmrn library
add_library(${PROJECT_NAME} ${SOURCES})

# Common openmrn includes
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${OPENMRNPATH}/src
        ${OPENMRNPATH}/include
)

include(${OPENMRNPATH}/cmake/spiffs.cmake)
