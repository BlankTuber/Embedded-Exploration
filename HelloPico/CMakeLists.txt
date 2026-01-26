cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico2)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(hello_pico C CXX ASM)

pico_sdk_init()

add_executable(hello_pico main.c)

target_link_libraries(hello_pico pico_stdlib)

pico_add_extra_outputs(hello_pico)

add_custom_target(flash
    COMMAND openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg
        -c "adapter speed 5000"
        -c "program hello_pico.elf verify reset exit"
    DEPENDS hello_pico
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
