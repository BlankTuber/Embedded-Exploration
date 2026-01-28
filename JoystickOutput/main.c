// This is built using the example from the pico-examples repository:
// https://github.com/raspberrypi/pico-examples/blob/master/adc/joystick_display/joystick_display.c
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define VSX_PIN 27
#define VSY_PIN 26
#define SW_PIN 22

int main() {
    // Initialize
    stdio_init_all();
    adc_init();

    adc_gpio_init(VSX_PIN);
    adc_gpio_init(VSY_PIN);

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

    // Read
    while (true) {
        bool is_pressed = gpio_get(SW_PIN);

        adc_select_input(0); // GP26
        uint adc_y_raw = adc_read();

        adc_select_input(1); // GP27
        uint adc_x_raw = adc_read();

        const uint bar_width = 40;
        const uint adc_max = (1 << 12) - 1; // 4095

        uint bar_x_pos = adc_x_raw * bar_width / adc_max;
        uint bar_y_pos = adc_y_raw * bar_width / adc_max;

        // Fancy print
        printf("X: [");
        for (uint i = 0; i < bar_width; i++) {
            putchar(i == bar_x_pos ? 'o' : ' ');
        }

        printf("] Y: [");
        for (uint i = 0; i < bar_width; i++) {
            putchar(i == bar_y_pos ? 'o' : ' ');
        }

        printf("] SW: %s\n", is_pressed ? "released" : "pressed");
        sleep_ms(50);
    }
}
