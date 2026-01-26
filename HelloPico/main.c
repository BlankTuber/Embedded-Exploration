// This program is based off the "blink_simple" example from the "pico-examples"
// repository:
// https://github.com/raspberrypi/pico-examples/blob/master/blink_simple/blink_simple.c
#include "pico/stdlib.h"

// Define LED delay in milliseconds
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 50
#endif

// Check if led exists
#ifndef PICO_DEFAULT_LED_PIN
#warning Hello Pico requires the board to have LED
#endif

// Initialize
void pico_led_init(void) {
#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
}

// Increase delay each time
void increase_delay(int *delay) {
    if (*delay >= 1000) {
        *delay = 0;
    }
    *delay += 50;
}

// Turn the LED on and off based on boolean value
void pico_set_led(bool led_on, int *delay) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
    if (delay != NULL) {
        increase_delay(delay);
    }
#endif
}

int main() {
    pico_led_init();
    int delay = LED_DELAY_MS;
    while (true) {
        pico_set_led(true, &delay);
        sleep_ms(delay);
        pico_set_led(false, NULL);
        sleep_ms(delay);
    }
}
