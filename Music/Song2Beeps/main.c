#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "tones.h"

#define BUZZER_PIN 15
#define PWM_CLOCK 1000000 // 1MHz

// Set tone
void set_tone(uint slice, uint freq) {
    if (freq == 0) {
        pwm_set_chan_level(slice, PWM_CHAN_B, 0);
    } else {
        // Find wrap based on frequency
        uint wrap = PWM_CLOCK / freq;
        pwm_set_wrap(slice, wrap);
        pwm_set_chan_level(slice, PWM_CHAN_B, wrap / 2);
    }
}

int main() {
    // Initialize pin
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);

    // 150MHz / 150 = 1MHz
    pwm_set_clkdiv(slice, 150.f);
    pwm_set_enabled(slice, true);

    // Get length of song
    int count = sizeof(tones) / sizeof(tones[0]);

    // Play song
    for (int i = 0; i < count; i++) {
        set_tone(slice, tones[i]);
        sleep_ms(TARGET_TONE_MS);
    }

    // Stop buzzer
    set_tone(slice, 0);
    pwm_set_enabled(slice, false);

    return 0;
}
