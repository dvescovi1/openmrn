# Utils library CMakeLists.txt

file(GLOB SOURCES
    "*.c"
    "*.cpp"
    "*.cxx"
)

add_library(utils STATIC ${SOURCES})

target_include_directories(utils PUBLIC 
    ${OPENMRNPATH}/include
)
