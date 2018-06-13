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

add_unit_test(NAME dejsonize_test LIBS pre::json)
add_unit_test(NAME variant_for_each_type LIBS pre::json)
