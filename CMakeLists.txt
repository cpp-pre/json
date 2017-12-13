cmake_minimum_required (VERSION 3.0.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.116.tar.gz"
    SHA1 "c872605eee88941a751fb91a028cc5a69c77eac0"
)

project (cpp-pre)

set (pre_VERSION_MAJOR 1)
set (pre_VERSION_MINOR 4)
set (pre_VERSION_PATCH 5)

set (PRE_VERSION "${pre_VERSION_MAJOR}.${pre_VERSION_MINOR}.${pre_VERSION_PATCH}")

# Compile unit tests
option(UNIT_TESTS "Enable Unit Testing" OFF)

if (UNIT_TESTS)
  enable_testing()
endif()

add_definitions(--std=c++11)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "CLANG")
	add_compile_options(-Wall
					-Werror
					-Wno-unused-local-typedefs
					-Wno-unused-variable
					-Wno-deprecated-declarations # neede for boost 1.59 filesystem uses auto_ptr :-( gcc5.3 does not like this
					-Wno-uninitialized
					)
endif()				

if(MSVC)
  add_definitions(-DNOMINMAX)
endif()

# Boost
if(MSVC)
  add_definitions(-DBOOST_ALL_NO_LIB=1)
endif()

set(Boost_USE_MULTITHREADED ON)
hunter_add_package(Boost)
find_package(Boost 1.59 REQUIRED)
include_directories ( ${Boost_INCLUDE_DIRS} )

include_directories( BEFORE ${CMAKE_CURRENT_LIST_DIR} )

# nlohmann-json
hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/pre DESTINATION include
        FILES_MATCHING PATTERN "*.[ih]pp")

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/mapbox DESTINATION include
        FILES_MATCHING PATTERN "*.[ih]pp")

include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR include/)
configure_package_config_file(
  ${cpp-pre_SOURCE_DIR}/cmake/modules/lib-${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION lib/cmake/lib-${PROJECT_NAME}-${PRE_VERSION}
  PATH_VARS INCLUDE_INSTALL_DIR)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config-version.cmake
    VERSION ${PRE_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lib-${PROJECT_NAME}-config-version.cmake
    DESTINATION lib/cmake/lib-${PROJECT_NAME}-${PRE_VERSION})

# Unit testing
if (UNIT_TESTS)
  add_subdirectory(test)
endif()
