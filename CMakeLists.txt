cmake_minimum_required(VERSION 3.20)
project(Vaslet VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)
option(VASLET_BUILD_EXAMPLES "Build client examples" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/vaslet)
install(FILES docs/api.md DESTINATION ${CMAKE_INSTALL_DOCDIR})

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
if(VASLET_BUILD_EXAMPLES)
  add_subdirectory(examples/client)
endif()
