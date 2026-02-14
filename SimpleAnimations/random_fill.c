#include "led_matrix.h"
#include <stdlib.h>

void random_fill(PIO pio, uint sm, uint32_t *grid) {
    int order[NUM_PIXELS];
    for (int i = 0; i < NUM_PIXELS; i++)
        order[i] = i;

    for (int i = NUM_PIXELS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }

    for (int i = 0; i < NUM_PIXELS; i++) {
        grid[order[i]] = convert_rgb(0x03, 0, 0x03);
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }

    for (int i = NUM_PIXELS - 1; i >= 0; i--) {
        grid[order[i]] = 0;
        put_pixels(pio, sm, grid);
        sleep_ms(10);
    }
}
