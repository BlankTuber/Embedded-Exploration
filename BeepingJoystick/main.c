#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

// Joystick
#define VSY_PIN 26
#define SW_PIN 22

// Passive Buzzer
#define PWM_PIN 15

void set_tone(uint slice, uint freq) {
    pwm_set_wrap(slice, freq);
    uint channel = pwm_gpio_to_channel(PWM_PIN);
    pwm_set_chan_level(slice, channel, freq / 2);
}

uint initialize() {
    adc_init();
    adc_gpio_init(VSY_PIN);

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    return slice_num;
}

uint read_adc() {
    adc_select_input(0);
    uint adc_y_raw = adc_read();

    // Make it something workable
    if (adc_y_raw < 400) {
        adc_y_raw = 400;
    } else if (adc_y_raw > 4000) {
        adc_y_raw = 4000;
    }

    return adc_y_raw;
}

int main() {
    // Initialize pins and get slice
    uint slice = initialize();

    // Set 1MHz base tone
    pwm_set_clkdiv(slice, 150.0f);

    // Enable pwm
    bool is_on = false;
    pwm_set_enabled(slice, is_on);

    while (true) {
        bool pressed = !gpio_get(SW_PIN);

        // Turn the buzzer on and off
        if (pressed) {
            is_on = !is_on;
            pwm_set_enabled(slice, is_on);

            // Wait for button release
            while (!gpio_get(SW_PIN)) {
                sleep_ms(10);
            }
        }

        uint frequency = read_adc();
        set_tone(slice, frequency);
        sleep_ms(50);
    }
}
