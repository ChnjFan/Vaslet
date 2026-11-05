# Check that a C++ caller can compile and link against the public API.
add_executable(vaslet_api_test unit/api_test.cpp)
target_link_libraries(vaslet_api_test PRIVATE vaslet_client)
add_test(NAME api_cpp_linkage COMMAND vaslet_api_test)
add_test(NAME command_line_smoke
  COMMAND ${CMAKE_COMMAND}
    "-DSERVER=$<TARGET_FILE:vasletd>"
    "-DCLI=$<TARGET_FILE:vasletctl>"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/integration/command_line.cmake")
