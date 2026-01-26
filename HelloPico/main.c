// This program is based off the "blink_simple" example from the "pico-examples"
// repository:
// https://github.com/raspberrypi/pico-examples/blob/master/blink_simple/blink_simple.c
#include "pico/stdlib.h"

// Define LED delay in milliseconds
#ifndef LED_DELAY_MS
#define LED_DELAY_MS 1000
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

// Turn the LED on and off based on boolean value
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
  gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

int main() {
  pico_led_init();
  while (true) {
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
  }
}
