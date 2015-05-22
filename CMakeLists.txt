cmake_minimum_required (VERSION 2.8.11)
project (cpp-pre)

set (pre_VERSION_MAJOR 1)
set (pre_VERSION_MINOR 0)
set (pre_VERSION_PATCH 0)

set (PRE_VERSION "${pre_VERSION_MAJOR}.${pre_VERSION_MINOR}.${pre_VERSION_PATCH}")

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

include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR include/)
configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/modules/lib-${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION lib/cmake/lib-${PROJECT_NAME}-${PRE_VERSION}
  PATH_VARS INCLUDE_INSTALL_DIR)

#configure_file(${CMAKE_SOURCE_DIR}/cmake/modules/lib-${PROJECT_NAME}-config.cmake.in
#    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config.cmake
#    @ONLY)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config-version.cmake
    VERSION ${PRE_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config-version.cmake
    DESTINATION lib/cmake/lib-${PROJECT_NAME}-${PRE_VERSION})
