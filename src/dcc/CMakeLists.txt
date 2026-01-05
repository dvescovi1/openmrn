# DCC library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

add_library(dcc STATIC ${SOURCES})

target_include_directories(dcc PUBLIC 
    ${OPENMRNPATH}/include
)

target_link_libraries(dcc PUBLIC
    utils
    executor
)
