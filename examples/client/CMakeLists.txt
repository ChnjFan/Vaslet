file(GLOB_RECURSE VASLET_EXAMPLE_SOURCES CONFIGURE_DEPENDS
    *.cpp
    *.h
)
add_executable(vaslet_client_example ${VASLET_EXAMPLE_SOURCES})
target_link_libraries(vaslet_client_example PRIVATE
    vaslet_client vaslet_build_options
)
