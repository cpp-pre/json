cmake_minimum_required (VERSION 2.8.11)
project (lib-cpp-pre)

# Boost
set(BOOST_ALL_DYN_LINK ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54 REQUIRED) 
include_directories ( ${Boost_INCLUDE_DIRS} )

include_directories( BEFORE ${CMAKE_CURRENT_LIST_DIR} )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/pre DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
