# \brief add_unit_test(NAME <test name> LIBS <libraries to link>
# A file <test name>.cpp has to exists for the test to be valid. 
# The main method <test name>.cpp should return non-zero on failure.
function(add_unit_test)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs LIBS)
  cmake_parse_arguments(add_unit_test "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(test_${add_unit_test_NAME} 
    ${add_unit_test_NAME}.cpp 
    $<TARGET_OBJECTS:SUBSYSCOM_SLC_SERVER_OBJECT_FILES_WITHOUT_MAIN>)

  target_link_libraries(test_${add_unit_test_NAME} 
    ${add_unit_test_LIBS})

  add_test(NAME test_${add_unit_test_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_${add_unit_test_NAME} 
        --catch_system_errors=no )

endfunction(add_unit_test) 

## Boost
find_package(Boost 1.55 COMPONENTS system thread log chrono filesystem  
  date_time iostreams log unit_test_framework REQUIRED) 
include_directories(AFTER ${Boost_INCLUDE_DIRS})
link_directories ( ${Boost_LIBRARY_DIRS} )

include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})

add_unit_test(NAME BasicSlcIdentificationTest 
  LIBS 
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${libWatchdogAccessor_LIBRARIES}
  subsyscom-base-server
  sauter-toolbox
  ucfg
  config
  rt)
