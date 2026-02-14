#include "led_matrix.h"

void ripple(PIO pio, uint sm, uint32_t *grid) {
    for (int i = 0; i < MATRIX_SIDE; i++) {
        uint32_t current_line_rgb = convert_rgb(0x05, 0x05, 0);
        uint32_t previous_line_rgb = convert_rgb(0x01, 0x01, 0);

        for (int j = 0; j < NUM_PIXELS; j++)
            grid[j] = 0;

        for (int y = 0; y <= i; y++)
            grid[xy(i, y)] = current_line_rgb;
        for (int x = 0; x < i; x++)
            grid[xy(x, i)] = current_line_rgb;

        if (i > 0) {
            for (int y = 0; y <= i - 1; y++)
                grid[xy(i - 1, y)] = previous_line_rgb;
            for (int x = 0; x < i - 1; x++)
                grid[xy(x, i - 1)] = previous_line_rgb;
        }

        put_pixels(pio, sm, grid);

        sleep_ms(100);
    }
}
