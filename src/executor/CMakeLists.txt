# Executor library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cpp"
)

add_library(executor STATIC ${SOURCES})

target_include_directories(executor PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(executor PUBLIC
    utils
    os
)
