# Console library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cpp"
)

add_library(console STATIC ${SOURCES})

target_include_directories(console PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(console PUBLIC
    utils
    os
)
