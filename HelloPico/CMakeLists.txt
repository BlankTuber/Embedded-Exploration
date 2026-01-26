cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(hello_pico C CXX ASM)

pico_sdk_init()

add_executable(hello_pico main.c)

target_link_libraries(hello_pico pico_stdlib)

pico_add_extra_outputs(hello_pico)
