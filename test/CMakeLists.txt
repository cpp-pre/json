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
find_package(Boost 1.59 COMPONENTS system thread log chrono filesystem  
  date_time iostreams log REQUIRED) 
include_directories(AFTER ${Boost_INCLUDE_DIRS})
link_directories ( ${Boost_LIBRARY_DIRS} )

include_directories(BEFORE ${CMAKE_SOURCE_DIR})

# nlohmann-json
hunter_add_package(nlohmann-json)
find_package(nlohmann-json REQUIRED)

add_unit_test(NAME asio_mockup_serial_port_service_test 
  LIBS 
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES})

add_unit_test(NAME adapted_struct_printer)

add_unit_test(NAME dejsonize_test
  LIBS nlohmann-json::nlohmann-json)
