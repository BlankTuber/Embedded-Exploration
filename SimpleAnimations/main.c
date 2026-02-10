#include <stdlib.h>

#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 256
#define PIN 16

typedef void (*animation_func)(PIO pio, uint sm, uint32_t *grid);

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t convert_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void snake(PIO pio, uint sm, uint32_t *grid) {
    const int snake_len = NUM_PIXELS * 0.05;
    for (int i = 0; i < NUM_PIXELS + snake_len; i++) {
        if (i < NUM_PIXELS)
            grid[i] = convert_rgb(0x10, 0, 0);
        if (i >= snake_len)
            grid[i - snake_len] = 0;
        for (int p = 0; p < NUM_PIXELS; p++) {
            put_pixel(pio, sm, grid[p]);
        }
        sleep_ms(10);
    }
}

int main() {
    PIO pio;
    uint sm;
    uint offset;

    animation_func animations[] = {snake};

    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset, PIN, 1, true);
    hard_assert(success);

    ws2812_program_init(pio, sm, offset, PIN, 800000, IS_RGBW);

    uint32_t grid[NUM_PIXELS] = {0};

    while (true) {
        int animation = rand() % count_of(animations);

        animations[animation](pio, sm, grid);

        for (int i = 0; i < NUM_PIXELS; i++) {
            grid[i] = 0;
            put_pixel(pio, sm, grid[i]);
        }
        sleep_ms(500);
    }

    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
}
