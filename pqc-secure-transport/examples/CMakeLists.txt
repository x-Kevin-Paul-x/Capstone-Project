cmake_minimum_required(VERSION 3.16)
project(pqc_examples C)

add_executable(secure_talker secure_talker.c)
target_include_directories(secure_talker PRIVATE ${CMAKE_SOURCE_DIR}/../include)
target_link_libraries(secure_talker PRIVATE pqc)

add_executable(secure_listener secure_listener.c)
target_include_directories(secure_listener PRIVATE ${CMAKE_SOURCE_DIR}/../include)
target_link_libraries(secure_listener PRIVATE pqc)
