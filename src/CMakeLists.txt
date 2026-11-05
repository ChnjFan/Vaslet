configure_file(common/version.h.in generated/vaslet_version.h @ONLY)

add_library(vaslet_client SHARED client/vaslet.cpp)
target_compile_features(vaslet_client PUBLIC cxx_std_17)
set_target_properties(vaslet_client PROPERTIES
  OUTPUT_NAME vaslet
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN YES)
target_include_directories(vaslet_client
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_executable(vasletd server/main.cpp)
target_include_directories(vasletd PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_executable(vasletctl cli/main.cpp)
target_link_libraries(vasletctl PRIVATE vaslet_client)
if(APPLE)
  set_target_properties(vasletctl PROPERTIES INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX)
  set_target_properties(vasletctl PROPERTIES INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

install(TARGETS vasletd vasletctl vaslet_client
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/vaslet" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
