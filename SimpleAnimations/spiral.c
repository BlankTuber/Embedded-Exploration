#include "led_matrix.h"

static void unspiral(PIO pio, uint sm, uint32_t *grid) {
    int x = 0, y = 0, dx = 0, dy = -1;
    int path[NUM_PIXELS];

    for (int i = 0; i < NUM_PIXELS; i++) {
        if (-MATRIX_SIDE / 2 < x && x <= MATRIX_SIDE / 2 &&
            -MATRIX_SIDE / 2 < y && y <= MATRIX_SIDE / 2) {
            path[i] = xy(x + MATRIX_SIDE / 2 - 1, y + MATRIX_SIDE / 2 - 1);
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

    for (int i = NUM_PIXELS - 1; i >= 0; i--) {
        if (path[i] >= 0)
            grid[path[i]] = 0;
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }
}

void spiral(PIO pio, uint sm, uint32_t *grid) {
    int x = 0, y = 0, dx = 0, dy = -1;

    for (int i = 0; i < NUM_PIXELS; i++) {
        if (-MATRIX_SIDE / 2 < x && x <= MATRIX_SIDE / 2 &&
            -MATRIX_SIDE / 2 < y && y <= MATRIX_SIDE / 2) {
            grid[xy(x + MATRIX_SIDE / 2 - 1, y + MATRIX_SIDE / 2 - 1)] =
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
