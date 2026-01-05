# WithRottle library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

add_library(withrottle STATIC ${SOURCES})

target_include_directories(withrottle PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(withrottle PUBLIC
    utils
    executor
    os
)
