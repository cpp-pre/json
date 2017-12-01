# \brief add_unit_test(NAME <test name> LIBS <libraries to link>
# A file <test name>.cpp has to exists for the test to be valid. 
# The main method <test name>.cpp should return non-zero on failure.
function(add_unit_test)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs LIBS)
  cmake_parse_arguments(add_unit_test "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(test_${add_unit_test_NAME} 
    ${add_unit_test_NAME}.cpp)

  target_link_libraries(test_${add_unit_test_NAME} 
    ${add_unit_test_LIBS})

  add_test(NAME test_${add_unit_test_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_${add_unit_test_NAME} )

endfunction(add_unit_test) 

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Disable auto_ptr errors
  add_definitions(-Wno-deprecated-declarations) 
endif()

## Multithreading support
find_package(Threads REQUIRED)

## Boost
set(Boost_USE_MULTITHREADED ON)

if (BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
  add_definitions(-DBOOST_ALL_DYN_LINK
                  -DBOOST_LOG_DYN_LINK
                  -DBOOST_TEST_DYN_LINK)
endif(BUILD_SHARED_LIBS)


hunter_add_package(Boost COMPONENTS system thread log chrono filesystem  
  date_time iostreams log)
find_package(Boost 1.58 COMPONENTS system thread log chrono filesystem
  date_time iostreams log REQUIRED) 
include_directories(AFTER ${Boost_INCLUDE_DIRS})
link_directories ( ${Boost_LIBRARY_DIRS} )

include_directories(BEFORE ${CMAKE_SOURCE_DIR})

# nlohmann-json
hunter_add_package(nlohmann_json)
find_package(nlohmann_json REQUIRED)

set (platform_specific_lib)
if(WIN32)
 add_definitions(-DWIN32_LEAN_AND_MEAN)
 set (platform_specific_lib ws2_32)
endif()

add_unit_test(NAME asio_mockup_serial_port_service_test 
  LIBS 
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${platform_specific_lib})

add_unit_test(NAME dejsonize_test
  LIBS nlohmann_json)

add_unit_test(NAME function_traits_test 
  LIBS 
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${platform_specific_lib})
  
add_unit_test(NAME retry_for
  LIBS ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES})

add_unit_test(NAME variant_for_each_type
  LIBS ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES})
