find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})


if(NOT DOXYGEN_FOUND)
	message(FATAL_ERROR "Doxygen is needed to build the documentation.")
else()
	message(STATUS "Doxygen found.")
endif()

message(STATUS  "Project source dir " ${PROJECT_SOURCE_DIR})

set(doxyfile_in ./Doxyfile.in)
set(doxyfile ../Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
