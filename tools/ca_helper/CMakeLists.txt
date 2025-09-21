cmake_minimum_required(VERSION 3.16)
project(ca_helper C)

add_executable(ca_helper ca_helper.c)
# Link to pqm4::pqm4 if available (pqm4 stub provides alias)
find_package(pqm4 QUIET)
if(TARGET pqm4::pqm4)
  target_link_libraries(ca_helper PRIVATE pqm4::pqm4)
  target_include_directories(ca_helper PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
else()
  message(STATUS "pqm4 not found: building ca_helper as a template that writes placeholders")
endif()

install(TARGETS ca_helper RUNTIME DESTINATION bin)
