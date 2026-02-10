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

static inline int xy(int x, int y) {
    if (y % 2 == 0)
        return y * 16 + x;
    else
        return y * 16 + (15 - x);
}

static inline void put_pixels(PIO pio, uint sm, uint32_t *grid) {
    for (int p = 0; p < NUM_PIXELS; p++) {
        put_pixel(pio, sm, grid[p]);
    }
}

void snake(PIO pio, uint sm, uint32_t *grid) {
    const int snake_len = NUM_PIXELS * 0.05;
    for (int i = 0; i < NUM_PIXELS + snake_len; i++) {
        if (i < NUM_PIXELS)
            grid[i] = convert_rgb(0x10, 0, 0);
        if (i >= snake_len)
            grid[i - snake_len] = 0;
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }
}

void unspiral(PIO pio, uint sm, uint32_t *grid) {
    int x = 0, y = 0, dx = 0, dy = -1;
    int side = 16;
    int path[NUM_PIXELS];

    // Record the spiral order
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (-side / 2 < x && x <= side / 2 && -side / 2 < y && y <= side / 2) {
            path[i] = xy(x + side / 2 - 1, y + side / 2 - 1);
        } else {
            path[i] = -1;
        }
        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1 - y)) {
            int temp = dx;
            dx = -dy;
            dy = temp;
        }
        x = x + dx;
        y = y + dy;
    }

    // Clear pixels in reverse spiral order
    for (int i = NUM_PIXELS - 1; i >= 0; i--) {
        if (path[i] >= 0)
            grid[path[i]] = 0;
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }
}

void spiral(PIO pio, uint sm, uint32_t *grid) {
    int x = 0;
    int y = 0;
    int dx = 0;
    int dy = -1;

    int side = 16;

    for (int i = 0; i < NUM_PIXELS; i++) {
        if (-side / 2 < x && x <= side / 2 && -side / 2 < y && y <= side / 2) {
            grid[xy(x + side / 2 - 1, y + side / 2 - 1)] =
                convert_rgb(0, 0x02, 0);
        }
        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1 - y)) {
            int temp = dx;
            dx = -dy;
            dy = temp;
        }
        x = x + dx;
        y = y + dy;
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }

    unspiral(pio, sm, grid);
}

// void raindrops(PIO pio, uint sm, uint32_t *grid) {}

int main() {
    PIO pio;
    uint sm;
    uint offset;

    animation_func animations[] = {snake, spiral};

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
