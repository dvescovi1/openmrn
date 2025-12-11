# OpenMRN Documentation

find_package(Doxygen)

if(DOXYGEN_FOUND)
    # Set Doxygen input and output directories
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/html)
    
    # Configure Doxyfile
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    # Check if Doxyfile.in exists, otherwise use a basic template
    if(EXISTS ${DOXYGEN_IN})
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    else()
        # Create a basic Doxyfile if one doesn't exist
        file(WRITE ${DOXYGEN_OUT}
"PROJECT_NAME           = \"OpenMRN\"
PROJECT_NUMBER         = \"${PROJECT_VERSION}\"
OUTPUT_DIRECTORY       = ${DOXYGEN_OUTPUT_DIR}
INPUT                  = ${DOXYGEN_INPUT_DIR}
RECURSIVE              = YES
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = NO
EXTRACT_STATIC         = YES
GENERATE_LATEX         = NO
GENERATE_HTML          = YES
HTML_OUTPUT            = .
"
        )
    endif()
    
    # Add custom target for documentation
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    
    # Install documentation
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        OPTIONAL
    )
    
    message(STATUS "Doxygen found - 'docs' target available")
else()
    message(STATUS "Doxygen not found - documentation will not be built")
endif()
