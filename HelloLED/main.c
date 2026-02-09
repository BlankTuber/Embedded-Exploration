// This code is based off the example from the "pico-examples" repository:
// https://github.com/raspberrypi/pico-examples/blob/master/pio/ws2812/ws2812.c

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 256
#define WS2812_PIN 16

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

int main() {
    stdio_init_all();

    PIO pio;
    uint sm;
    uint offset;

    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    hard_assert(success);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    uint32_t grid[NUM_PIXELS] = {0};
    while (true) {
        for (int i = 0; i < NUM_PIXELS; i++) {
            grid[i] = urgb_u32(0xff, 0, 0);
            if (i > 10) {
                grid[i - 10] = 0;
            }

            for (int pixel = 0; pixel < NUM_PIXELS; ++pixel) {
                put_pixel(pio, sm, grid[pixel]);
            }

            sleep_ms(10);
        }

        for (int i = 0; i < NUM_PIXELS; ++i) {
            grid[i] = 0;
        }
    }

    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
