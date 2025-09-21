cmake_minimum_required(VERSION 3.16)
project(pqc_tests C)

add_executable(test_pqc test_main.c)
target_include_directories(test_pqc PRIVATE ${CMAKE_SOURCE_DIR}/../include)
target_link_libraries(test_pqc PRIVATE pqc)

enable_testing()
add_test(NAME test_pqc COMMAND test_pqc)
