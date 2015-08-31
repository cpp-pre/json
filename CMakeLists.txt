cmake_minimum_required (VERSION 2.8.11)

include("cmake/HunterGate.cmake")
HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.12.5.tar.gz"
  SHA1 "a54fad3f61e35ba43ccede0c57c1a12e37e95e5f"
)

project (cpp-pre)
enable_testing()

set (pre_VERSION_MAJOR 1)
set (pre_VERSION_MINOR 2)
set (pre_VERSION_PATCH 2)

set (PRE_VERSION "${pre_VERSION_MAJOR}.${pre_VERSION_MINOR}.${pre_VERSION_PATCH}")

add_definitions(--std=c++11)

# Boost
set(Boost_USE_MULTITHREADED ON)
hunter_add_package(Boost)
find_package(Boost 1.54 REQUIRED) 
include_directories ( ${Boost_INCLUDE_DIRS} )

include_directories( BEFORE ${CMAKE_CURRENT_LIST_DIR} )

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/pre DESTINATION include
        FILES_MATCHING PATTERN "*.[ih]pp")

include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR include/)
configure_package_config_file(
  ${cpp-pre_SOURCE_DIR}/cmake/modules/lib-${PROJECT_NAME}-config.cmake.in
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

# Unit testing
add_subdirectory(test)
