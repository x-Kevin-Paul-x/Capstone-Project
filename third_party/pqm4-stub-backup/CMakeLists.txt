# pqm4 CMake stub
# This CMakeLists provides a stub target `pqm4::pqm4` when pqm4 sources are not present.
# Replace or remove this file when you add the real pqm4 source tree as a submodule.

if(NOT TARGET pqm4::pqm4)
  add_library(pqm4_stub INTERFACE)
  add_library(pqm4::pqm4 ALIAS pqm4_stub)
  target_include_directories(pqm4_stub INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  message(STATUS "Using pqm4 stub. Add real pqm4 sources under third_party/pqm4 or add as a submodule.")
endif()

# Optionally provide helpers or imported targets for specific algorithms in future.