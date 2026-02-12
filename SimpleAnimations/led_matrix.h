#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "hardware/pio.h"
#include "pico/stdlib.h"

#define IS_RGBW false
#define NUM_PIXELS 256
#define MATRIX_SIDE 16
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
        return y * MATRIX_SIDE + x;
    else
        return y * MATRIX_SIDE + (MATRIX_SIDE - 1 - x);
}

static inline void put_pixels(PIO pio, uint sm, uint32_t *grid) {
    for (int p = 0; p < NUM_PIXELS; p++) {
        put_pixel(pio, sm, grid[p]);
    }
}

#endif
