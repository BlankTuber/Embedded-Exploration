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

static inline uint32_t hsv_to_rgb(int hue, uint8_t brightness) {
    int region = hue / 60;
    int remainder = (hue % 60) * brightness / 60;

    uint8_t p = 0;
    uint8_t q = brightness - remainder;
    uint8_t t = remainder;

    switch (region) {
    case 0:
        return convert_rgb(brightness, t, p);
    case 1:
        return convert_rgb(q, brightness, p);
    case 2:
        return convert_rgb(p, brightness, t);
    case 3:
        return convert_rgb(p, q, brightness);
    case 4:
        return convert_rgb(t, p, brightness);
    default:
        return convert_rgb(brightness, p, q);
    }
}

#endif
