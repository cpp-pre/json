add_executable(from_json from_json.cpp)
target_link_libraries(from_json pre::json)

add_executable(to_json to_json.cpp)
target_link_libraries(to_json pre::json)

add_test(NAME from_json
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/from_json )

add_test(NAME to_json
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/from_json )
