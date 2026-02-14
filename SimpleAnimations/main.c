#include <stdlib.h>

#include "hardware/clocks.h"
#include "ws2812.pio.h"

#include "animations.h"

int main() {
    PIO pio;
    uint sm;
    uint offset;

    animation_func animations[] = {snake,  spiral,      raindrops,
                                   ripple, random_fill, rainbow_ripple};

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
