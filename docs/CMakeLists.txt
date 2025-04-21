find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    # Add custom target for generating documentation
    add_custom_target(gen_doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
