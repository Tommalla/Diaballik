cmake_minimum_required (VERSION 2.8)

project (Diaballik)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_CXX_FLAGS "-Wall -ggdb -std=c++11")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")

find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

#we want this version of DiaballikEngine to build first!
add_subdirectory(DiaballikEngine)
add_subdirectory(DiaballikBot)
#add_subdirectory(NullAI)
add_subdirectory (src)

#installation
set (CMAKE_INSTALL_PREFIX $ENV{HOME})
install (FILES ${PROJECT_BINARY_DIR}/Diaballik ${PROJECT_SOURCE_DIR}/config.ini DESTINATION .diaballik/ )
install (DIRECTORY ${PROJECT_SOURCE_DIR}/graphics DESTINATION .diaballik )
install (DIRECTORY ${PROJECT_SOURCE_DIR}/bots DESTINATION .diaballik)
install (PROGRAMS ${PROJECT_BINARY_DIR}/DiaballikBot/DiaballikBot DESTINATION .diaballik/bots/MCTSAI/ RENAME engine)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)
