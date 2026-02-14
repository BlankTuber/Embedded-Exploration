#include "led_matrix.h"

void rainbow_ripple(PIO pio, uint sm, uint32_t *grid) {
    for (int i = 0; i < MATRIX_SIDE; i++) {
        int hue = 360 * i / MATRIX_SIDE;
        for (int x = 0; x <= i; x++)
            grid[xy(x, i)] = hsv_to_rgb(hue, 0x15);
        for (int y = 0; y < i; y++)
            grid[xy(i, y)] = hsv_to_rgb(hue, 0x15);

        put_pixels(pio, sm, grid);
        sleep_ms(100);
    }

    int frames = 100;
    for (int f = 0; f < frames; f++) {
        for (int i = 0; i < MATRIX_SIDE; i++) {
            int hue = (360 * i / MATRIX_SIDE + f * 5) % 360;
            for (int x = 0; x <= i; x++)
                grid[xy(x, i)] = hsv_to_rgb(hue, 0x15);
            for (int y = 0; y < i; y++)
                grid[xy(i, y)] = hsv_to_rgb(hue, 0x15);
        }
        put_pixels(pio, sm, grid);
        sleep_ms(30);
    }

    for (int i = 0; i < MATRIX_SIDE; i++) {
        int hue = 360 * i / MATRIX_SIDE;
        for (int x = 0; x <= i; x++)
            grid[xy(x, i)] = 0;
        for (int y = 0; y < i; y++)
            grid[xy(i, y)] = 0;

        put_pixels(pio, sm, grid);
        sleep_ms(100);
    }
}
