#include "led_matrix.h"
#include <stdlib.h>

void raindrops(PIO pio, uint sm, uint32_t *grid) {
    int xy_array[MATRIX_SIDE][2];
    const int trail_len = 4;
    const int total_frames = 250;

    for (int i = 0; i < MATRIX_SIDE; i++) {
        xy_array[i][0] = -1;
        xy_array[i][1] = 0;
    }

    for (int frame = 0; frame < total_frames; frame++) {
        if (rand() % 3 == 0) {
            for (int i = 0; i < MATRIX_SIDE; i++) {
                if (xy_array[i][0] == -1) {
                    xy_array[i][0] = rand() % MATRIX_SIDE;
                    xy_array[i][1] = 0;
                    break;
                }
            }
        }

        for (int i = 0; i < MATRIX_SIDE; i++) {
            if (xy_array[i][0] == -1)
                continue;

            int head_x = xy_array[i][0];
            int head_y = xy_array[i][1];

            int tail_y = head_y - trail_len;
            if (tail_y >= 0 && tail_y < MATRIX_SIDE)
                grid[xy(head_x, tail_y)] = 0;

            for (int t = 0; t < trail_len; t++) {
                int pos_y = head_y - t;
                if (pos_y >= 0 && pos_y < MATRIX_SIDE) {
                    uint8_t brightness = 0x25 >> t;
                    grid[xy(head_x, pos_y)] = convert_rgb(0, 0, brightness);
                }
            }

            xy_array[i][1] += 1;

            if (head_y - trail_len >= MATRIX_SIDE) {
                xy_array[i][0] = -1;
            }
        }

        put_pixels(pio, sm, grid);
        sleep_ms(20);
    }
}
