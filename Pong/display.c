#include "display.h"

LED_Info initialize_led_panel() {
    LED_Info info;

    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &info.pio, &info.sm, &info.offset, LED_PIN, 1, true);
    hard_assert(success);

    ws2812_program_init(info.pio, info.sm, info.offset, LED_PIN, 800000,
                        IS_RGBW);

    return info;
}

void deinitialize_led_panel(LED_Info info) {
    pio_remove_program_and_unclaim_sm(&ws2812_program, info.pio, info.sm,
                                      info.offset);
}

void put_pixels(PIO pio, uint sm, uint32_t *grid) {
    for (int p = 0; p < NUM_PIXELS; p++) {
        put_pixel(pio, sm, grid[p]);
    }
}

uint32_t hsv_to_rgb(int hue, uint8_t brightness) {
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

void clear_grid(uint32_t *grid) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        grid[i] = 0;
    }
}
