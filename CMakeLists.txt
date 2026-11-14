cmake_minimum_required(VERSION 3.16)
project(vaslet
    VERSION 0.1.0
    DESCRIPTION "A lightweight JSON configuration center for Linux"
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

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND
   NOT DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE AND
   NOT DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT VASLET_IPO_SUPPORTED LANGUAGES CXX)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE "${VASLET_IPO_SUPPORTED}")
endif()

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
# Server language policy and mandatory Release options
# ---------------------------------------------------------------------------
add_library(vaslet_server_release_options INTERFACE)
target_compile_definitions(vaslet_server_release_options INTERFACE
    "$<$<CONFIG:Release>:VASLETD_RELEASE_BUILD=1>"
)
if(CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
    target_compile_options(vaslet_server_release_options INTERFACE
        -fno-exceptions -fno-rtti
        "$<$<CONFIG:Release>:-Os;-fvisibility=hidden;-fvisibility-inlines-hidden>"
    )
    # GCC and Clang expose no predefined macro for default symbol visibility.
    # Keep this build-contract marker coupled to the flags above.
    target_compile_definitions(vaslet_server_release_options INTERFACE
        "$<$<CONFIG:Release>:VASLETD_HIDDEN_VISIBILITY=1>"
    )
endif()

# ---------------------------------------------------------------------------
# Source modules
# ---------------------------------------------------------------------------
configure_file(common/version.h.in generated/vaslet_version.h @ONLY)
file(GLOB_RECURSE VASLET_COMMON_SOURCES CONFIGURE_DEPENDS
    common/*.cpp
    common/*.h
)
add_subdirectory(src)

# ---------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------
option(BUILD_TESTING "Build the Vaslet test suite" OFF)
if(BUILD_TESTING)
    enable_testing()
endif()

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
option(VASLET_BUILD_EXAMPLES "Build Vaslet examples" OFF)
if(VASLET_BUILD_EXAMPLES)
endif()

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
install(TARGETS vasletd
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/vaslet"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/vaslet)
install(FILES thirdparty/rapidjson/license.txt
    DESTINATION ${CMAKE_INSTALL_DATADIR}/vaslet
    RENAME rapidjson-LICENSE.txt
)
