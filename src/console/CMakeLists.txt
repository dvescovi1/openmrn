# Console library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

add_library(console STATIC ${SOURCES})

target_include_directories(console PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(console PUBLIC
    utils
    os
)
