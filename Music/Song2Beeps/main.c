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

// Play each sample's 3 tones in rapid succession for a smooth arpeggiation
void play_sample(uint slice, const int sample[]) {
    bool silent = true;
    for (int i = 0; i < MAX_TONES; i++) {
        if (sample[i] != 0)
            silent = false;
    }

    if (silent) {
        set_tone(slice, 0);
        sleep_ms(SAMPLE_DURATION_MS);
        return;
    }

    int elapsed = 0;
    while (elapsed < SAMPLE_DURATION_MS) {
        for (int i = 0; i < MAX_TONES && elapsed < SAMPLE_DURATION_MS; i++) {
            if (sample[i] == 0) {
                continue; // Skip
            }
            set_tone(slice, sample[i]);
            sleep_ms(TONE_SLICE_MS);
            elapsed += TONE_SLICE_MS;
        }
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
        play_sample(slice, tones[i]);
    }

    // Stop buzzer
    set_tone(slice, 0);

    return 0;
}
