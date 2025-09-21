cmake_minimum_required(VERSION 3.16)
project(pqc-secure-transport LANGUAGES C)

# Options
option(BUILD_EXAMPLES "Build example secure_talker/secure_listener" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_BENCH "Build benchmarks" ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Library
add_library(pqc
  src/pqc.c
)

target_include_directories(pqc
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compile definitions for hardening (can be adjusted per platform)
target_compile_options(pqc PRIVATE
  $<$<C_COMPILER_ID:GNU>:-Wall -Wextra -O2 -fno-builtin -fwrapv -fno-tree-vectorize>
  $<$<C_COMPILER_ID:Clang>:-Wall -Wextra -O2 -fno-builtin -fwrapv -fno-tree-vectorize>
)

# Examples
if (BUILD_EXAMPLES)
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt)
    add_subdirectory(examples)
  else()
    message(STATUS "Examples directory not found; skip add_subdirectory(examples). Create examples/CMakeLists.txt and example sources to enable.")
  endif()
endif()

# Tests
if (BUILD_TESTS)
  enable_testing()
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
  else()
    message(STATUS "Tests directory not found; skip add_subdirectory(tests). Create tests/CMakeLists.txt to enable.")
  endif()
endif()

# Benchmarks
if (BUILD_BENCH)
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bench/CMakeLists.txt)
    add_subdirectory(bench)
  else()
    message(STATUS "Bench directory not found; skip add_subdirectory(bench). Create bench/CMakeLists.txt to enable.")
  endif()
endif()

# Installation (minimal)
include(GNUInstallDirs)
install(TARGETS pqc
  EXPORT pqcTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export config
install(EXPORT pqcTargets
  FILE pqcTargets.cmake
  NAMESPACE pqc::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pqc
)

# Package-friendly variables
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/pqcConfigVersion.cmake"
  VERSION 0.1.0
  COMPATIBILITY AnyNewerVersion
)

# Top-level helpful message
message(STATUS "pqc-secure-transport CMake scaffold created.")
message(STATUS "Create the following directories and files to complete the scaffold:")
message(STATUS "  - include/pqc.h")
message(STATUS "  - src/pqc.c")
message(STATUS "  - examples/CMakeLists.txt + examples/secure_talker.c + examples/secure_listener.c")
message(STATUS "  - tests/CMakeLists.txt + tests/test_main.c")
message(STATUS "  - bench/CMakeLists.txt + bench/bench_main.c")
message(STATUS "  - docs/ADRs/ (ADR templates)")
