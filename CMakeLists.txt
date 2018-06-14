
# This is an #inglued <> generated CMakeLists.txt (https://github.com/header-only/inglued)
# To modify it edit CMakeLists.txt.tpl ( i.e. Generate it with `inglued cmaketpl` )


# This is an #inglued <> template !
#
# The template is processed by http://mustache.github.io/, more info about
# syntax here : http://mustache.github.io/mustache.5.html 
#
# You can access the following variables : 
# * pre : github organization name
# * json : current project name
# * . : current project srcs folder.
#
# *                                        : all deps direct and transitive 
#   -  : The cmake package name from the cmake_package_map otherwise: pre
#   -  : The cmake target name from cmake_package_map otherwise: ::
#   - pre : the github organization name
#   -  : the dependency repository name
#   -  : tag or branch wished for the dep
#   -  : the path you specified in deps/inglued -I
#   -  : same as above but with a guaranteed end slash.
#

cmake_minimum_required(VERSION 3.5.0)

project(json VERSION "2.0.0")
enable_testing()

# Compile with shipped-with headers or without 
option(INGLUED "Enable use of #inglued shipped with dependencies." ON)

# Compile unit tests
option(UNIT_TESTS "Enable Unit Testing" OFF)


# Warning as errors to ensure json quality
string(TOUPPER "${CMAKE_CXX_COMPILER_ID}" COMPILER_IN_USE)
if ("${COMPILER_IN_USE}" STREQUAL "GNU" OR "${COMPILER_IN_USE}" MATCHES "CLANG")
	add_definitions(
    -Wall
		-Werror
		-Wno-unused-local-typedefs
		-Wno-unused-variable
  )
endif()



# algorithm
if (INGLUED)
  include_directories(AFTER deps/boostorg/algorithm/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# array
if (INGLUED)
  include_directories(AFTER deps/boostorg/array/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# assert
if (INGLUED)
  include_directories(AFTER deps/boostorg/assert/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# bind
if (INGLUED)
  include_directories(AFTER deps/boostorg/bind/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# concept_check
if (INGLUED)
  include_directories(AFTER deps/boostorg/concept_check/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# config
if (INGLUED)
  include_directories(AFTER deps/boostorg/config/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# container_hash
if (INGLUED)
  include_directories(AFTER deps/boostorg/container_hash/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# core
if (INGLUED)
  include_directories(AFTER deps/boostorg/core/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# detail
if (INGLUED)
  include_directories(AFTER deps/boostorg/detail/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# exception
if (INGLUED)
  include_directories(AFTER deps/boostorg/exception/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# function
if (INGLUED)
  include_directories(AFTER deps/boostorg/function/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# function_types
if (INGLUED)
  include_directories(AFTER deps/boostorg/function_types/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# functional
if (INGLUED)
  include_directories(AFTER deps/boostorg/functional/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# fusion
if (INGLUED)
  include_directories(AFTER deps/boostorg/fusion/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# integer
if (INGLUED)
  include_directories(AFTER deps/boostorg/integer/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# io
if (INGLUED)
  include_directories(AFTER deps/boostorg/io/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# iterator
if (INGLUED)
  include_directories(AFTER deps/boostorg/iterator/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# move
if (INGLUED)
  include_directories(AFTER deps/boostorg/move/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# mpl
if (INGLUED)
  include_directories(AFTER deps/boostorg/mpl/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# numeric_conversion
if (INGLUED)
  include_directories(AFTER deps/boostorg/numeric_conversion/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# optional
if (INGLUED)
  include_directories(AFTER deps/boostorg/optional/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# predef
if (INGLUED)
  include_directories(AFTER deps/boostorg/predef/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# preprocessor
if (INGLUED)
  include_directories(AFTER deps/boostorg/preprocessor/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# range
if (INGLUED)
  include_directories(AFTER deps/boostorg/range/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# regex
if (INGLUED)
  include_directories(AFTER deps/boostorg/regex/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# smart_ptr
if (INGLUED)
  include_directories(AFTER deps/boostorg/smart_ptr/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# static_assert
if (INGLUED)
  include_directories(AFTER deps/boostorg/static_assert/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# system
if (INGLUED)
  include_directories(AFTER deps/boostorg/system/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# test
if (INGLUED)
  include_directories(AFTER deps/boostorg/test/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# throw_exception
if (INGLUED)
  include_directories(AFTER deps/boostorg/throw_exception/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# timer
if (INGLUED)
  include_directories(AFTER deps/boostorg/timer/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# tuple
if (INGLUED)
  include_directories(AFTER deps/boostorg/tuple/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# type_index
if (INGLUED)
  include_directories(AFTER deps/boostorg/type_index/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# type_traits
if (INGLUED)
  include_directories(AFTER deps/boostorg/type_traits/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# typeof
if (INGLUED)
  include_directories(AFTER deps/boostorg/typeof/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# utility
if (INGLUED)
  include_directories(AFTER deps/boostorg/utility/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# winapi
if (INGLUED)
  include_directories(AFTER deps/boostorg/winapi/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# json
if (INGLUED)
  include_directories(AFTER deps/nlohmann/json/include/)
else ()
  find_package(nlohmann_json REQUIRED)
endif()


# Define header only library
include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})
add_library(json INTERFACE)
add_library(pre::json ALIAS json)

set(include_install_dir "include")

if (INGLUED)

  target_include_directories(json INTERFACE 
    # Transitive libraries location once installed
    $<INSTALL_INTERFACE:${include_install_dir}/./deps> 
  )


else()

  target_link_libraries(json INTERFACE 
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      Boost::boost
      nlohmann_json
  )

endif()


if( UNIT_TESTS )
 add_subdirectory(examples)
 add_subdirectory(test)
endif()

# IDE Supports (XCode, VS, QtCreator they don't list headers)
file (GLOB_RECURSE HPP_FILES ./*.h*)
add_custom_target(sources SOURCES ${HPP_FILES})

# Installing

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/modules/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)


# Targets:
install(
    TARGETS json
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
    DIRECTORY .
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.[ih]*"
)

# Install the deps when run in INGLUED mode
if (INGLUED)


  install(
      DIRECTORY deps/boostorg/algorithm/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/algorithm/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/array/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/array/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/assert/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/assert/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/bind/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/bind/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/concept_check/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/concept_check/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/config/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/config/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/container_hash/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/container_hash/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/core/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/core/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/detail/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/detail/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/exception/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/exception/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/function/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/function/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/function_types/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/function_types/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/functional/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/functional/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/fusion/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/fusion/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/integer/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/integer/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/io/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/io/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/iterator/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/iterator/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/move/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/move/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/mpl/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/mpl/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/numeric_conversion/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/numeric_conversion/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/optional/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/optional/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/predef/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/predef/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/preprocessor/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/preprocessor/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/range/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/range/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/regex/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/regex/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/smart_ptr/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/smart_ptr/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/static_assert/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/static_assert/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/system/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/system/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/test/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/test/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/throw_exception/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/throw_exception/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/timer/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/timer/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/tuple/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/tuple/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/type_index/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/type_index/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/type_traits/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/type_traits/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/typeof/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/typeof/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/utility/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/utility/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/boostorg/winapi/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/winapi/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/nlohmann/json/include/
      DESTINATION ${include_install_dir}/./deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/nlohmann/json/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


endif(INGLUED)

# Config
#   * <prefix>/lib/cmake/json/jsonConfig.cmake
#   * <prefix>/lib/cmake/json/jsonConfigVersion.cmake
#   * <prefix>/lib/cmake/json/jsonTargets.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

