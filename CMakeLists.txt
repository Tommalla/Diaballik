cmake_minimum_required (VERSION 2.8)

project (Diaballik)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set (CMAKE_CXX_FLAGS "-O2 -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")

find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

#we want this version of DiaballikEngine to build first!
add_subdirectory(DiaballikEngine)
add_subdirectory(DiaballikBot)
#add_subdirectory(NullAI)
add_subdirectory (src)
