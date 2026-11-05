# ---------------------------------------------------------------------------
# GoogleTest dependency
# ---------------------------------------------------------------------------
include(FetchContent)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# Test dependencies stay within this subtree and are never installed.
set(BUILD_SHARED_LIBS OFF)
set(BUILD_GMOCK OFF CACHE BOOL "Build GoogleMock" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Install GoogleTest" FORCE)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
    URL_HASH SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# ---------------------------------------------------------------------------
# Public API tests
# ---------------------------------------------------------------------------
file(GLOB_RECURSE VASLET_API_TEST_SOURCES CONFIGURE_DEPENDS
    unit/api/*.cpp
    unit/api/*.h
)
add_executable(vaslet_api_test ${VASLET_API_TEST_SOURCES})
target_link_libraries(vaslet_api_test PRIVATE
    vaslet_client GTest::gtest_main vaslet_build_options
)
gtest_discover_tests(vaslet_api_test DISCOVERY_MODE PRE_TEST)

# ---------------------------------------------------------------------------
# Command-line parser tests
# ---------------------------------------------------------------------------
file(GLOB_RECURSE VASLET_COMMAND_LINE_TEST_SOURCES CONFIGURE_DEPENDS
    unit/command_line/*.cpp
    unit/command_line/*.h
)
add_executable(vaslet_command_line_test ${VASLET_COMMAND_LINE_TEST_SOURCES})
target_link_libraries(vaslet_command_line_test PRIVATE
    vaslet_command_line GTest::gtest_main vaslet_build_options
)
gtest_discover_tests(vaslet_command_line_test DISCOVERY_MODE PRE_TEST)

# ---------------------------------------------------------------------------
# Command-line integration checks
# ---------------------------------------------------------------------------
add_test(NAME command_line_smoke
    COMMAND ${CMAKE_COMMAND}
        "-DSERVER=$<TARGET_FILE:vasletd>"
        "-DCLI=$<TARGET_FILE:vasletctl>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/integration/command_line.cmake"
)
