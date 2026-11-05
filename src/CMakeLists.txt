# ---------------------------------------------------------------------------
# Generated version header
# ---------------------------------------------------------------------------
configure_file(common/version.h.in generated/vaslet_version.h @ONLY)

# ---------------------------------------------------------------------------
# Internal shared modules
# ---------------------------------------------------------------------------
file(GLOB_RECURSE VASLET_COMMON_SOURCES CONFIGURE_DEPENDS
    common/*.cpp
    common/*.h
)
add_library(vaslet_command_line STATIC ${VASLET_COMMON_SOURCES})
target_include_directories(vaslet_command_line PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(vaslet_command_line PRIVATE vaslet_build_options)

# ---------------------------------------------------------------------------
# Public client library
# ---------------------------------------------------------------------------
file(GLOB_RECURSE VASLET_CLIENT_SOURCES CONFIGURE_DEPENDS
    libvaslet/*.cpp
    libvaslet/*.h
    "${PROJECT_SOURCE_DIR}/include/vaslet/*.h"
)
add_library(vaslet_client SHARED ${VASLET_CLIENT_SOURCES})
set_target_properties(vaslet_client PROPERTIES
    OUTPUT_NAME vaslet
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)
target_include_directories(vaslet_client
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated"
)
target_link_libraries(vaslet_client PRIVATE vaslet_build_options)

# ---------------------------------------------------------------------------
# Configuration service
# ---------------------------------------------------------------------------
file(GLOB_RECURSE VASLET_SERVER_SOURCES CONFIGURE_DEPENDS
    vasletd/*.cpp
    vasletd/*.h
)
add_executable(vasletd ${VASLET_SERVER_SOURCES})
target_include_directories(vasletd PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_link_libraries(vasletd PRIVATE vaslet_command_line vaslet_build_options)

# ---------------------------------------------------------------------------
# Command-line client
# ---------------------------------------------------------------------------
file(GLOB_RECURSE VASLET_CLI_SOURCES CONFIGURE_DEPENDS
    vasletctl/*.cpp
    vasletctl/*.h
)
add_executable(vasletctl ${VASLET_CLI_SOURCES})
target_link_libraries(vasletctl PRIVATE
    vaslet_client vaslet_command_line vaslet_build_options
)
if(APPLE)
    set_target_properties(vasletctl PROPERTIES
        INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(UNIX)
    set_target_properties(vasletctl PROPERTIES
        INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()
