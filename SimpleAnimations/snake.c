#include "led_matrix.h"

void snake(PIO pio, uint sm, uint32_t *grid) {
    int snake_len = NUM_PIXELS * 0.05;
    if (snake_len < 1)
        snake_len = 1;
    for (int i = 0; i < NUM_PIXELS + snake_len; i++) {
        if (i < NUM_PIXELS)
            grid[i] = convert_rgb(0x10, 0, 0);
        if (i >= snake_len)
            grid[i - snake_len] = 0;
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }
}
