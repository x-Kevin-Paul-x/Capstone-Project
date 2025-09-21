cmake_minimum_required(VERSION 3.16)
project(pqc_bench C)

add_executable(bench_main bench_main.c)
target_include_directories(bench_main PRIVATE ${CMAKE_SOURCE_DIR}/../include)
target_link_libraries(bench_main PRIVATE pqc)
