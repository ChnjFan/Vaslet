cmake_minimum_required(VERSION 3.20)
project(Vaslet
    VERSION 0.1.0
    DESCRIPTION "A lightweight JSON configuration center for embedded Linux"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Global settings
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
    )
endif()

include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Compiler warnings for project targets
# ---------------------------------------------------------------------------
add_library(vaslet_build_options INTERFACE)
if(MSVC)
    target_compile_options(vaslet_build_options INTERFACE /W4 /WX)
else()
    target_compile_options(vaslet_build_options INTERFACE
        -Wall -Wextra -Wpedantic -Werror
    )
endif()

# ---------------------------------------------------------------------------
# Source modules
# ---------------------------------------------------------------------------
add_subdirectory(src)

# ---------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------
option(BUILD_TESTING "Build the Vaslet test suite" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
option(VASLET_BUILD_EXAMPLES "Build client examples" OFF)
if(VASLET_BUILD_EXAMPLES)
    add_subdirectory(examples/client)
endif()

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
install(TARGETS vasletd vasletctl vaslet_client
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/vaslet"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/vaslet)
install(FILES docs/api.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
