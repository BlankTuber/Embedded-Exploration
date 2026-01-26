cmake_minimum_required(VERSION 3.13)

# Project config
set(PROJECT_NAME "my_project")
set(PICO_BOARD pico2)

# SDK setup (Do Not Touch)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(${PROJECT_NAME} C CXX ASM)
set(PICOTOOL_FETCH_FROM_GIT OFF)
pico_sdk_init()

# Source files
add_executable(${PROJECT_NAME}
    main.c
)

# Libraries
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    # hardware_pwm      # Buzzer tones
    # hardware_adc      # Joystick analog
    # hardware_pio      # WS2812B LEDs
    # hardware_i2c
    # hardware_spi
)

# printf over USB (serial port)
pico_enable_stdio_usb(${PROJECT_NAME} 1)
# printf over UART (TX/RX pins)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Output files (Do Not Touch)
pico_add_extra_outputs(${PROJECT_NAME})

# Flash via debug probe (Do Not Touch)
add_custom_target(flash
    COMMAND openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg
        -c "adapter speed 5000"
        -c "program ${PROJECT_NAME}.elf verify reset exit"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
