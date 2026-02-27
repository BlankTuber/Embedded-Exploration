#ifndef DISPLAY_H
#define DISPLAY_H

#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 256
#define MATRIX_SIDE 16
#define LED_PIN 16

typedef struct {
    PIO pio;
    uint sm;
    uint offset;
} LED_Info;

LED_Info initialize_led_panel();
void deinitialize_led_panel(LED_Info info);

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

void clear_grid(uint32_t *grid);
void put_pixels(PIO pio, uint sm, uint32_t *grid);
uint32_t hsv_to_rgb(int hue, uint8_t brightness);

#endif
