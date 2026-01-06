# OS library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cpp"
)

add_library(os STATIC ${SOURCES})

target_include_directories(os PUBLIC 
    ${OPENMRNPATH}/include
)

# Link against FreeRTOS if available
if(TARGET freertos)
    target_link_libraries(os PUBLIC freertos)
endif()
