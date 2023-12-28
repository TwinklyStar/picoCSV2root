#---------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(CSV2root)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_INSTALL_PREFIX ..)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE(ROOT COMPONENTS Geom TMVA Eve Gui RGL EG)
IF (ROOT_FOUND)
    INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR})
    LINK_LIBRARIES(${ROOT_LIBRARIES})
    LINK_DIRECTORIES(${ROOT_LIBRARY_DIR})
ENDIF ()


include_directories(./)

file(GLOB files ./CSV2root.cpp)

add_executable(CSV2root ${files})

#----------------------------------------------------------------------------
# Install the executable to the directory of CMAKE_INSTALL_PREFIX
#
install(TARGETS CSV2root DESTINATION)
