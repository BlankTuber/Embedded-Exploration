// This is based off the example project "hello_pwm" here:
// "https://github.com/raspberrypi/pico-examples/blob/master/pwm/hello_pwm/hello_pwm.c"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define BUZZER_PIN 28

// Update tone
void set_tone(uint slice, uint newwrap) {
    pwm_set_wrap(slice, newwrap);
    pwm_set_chan_level(slice, PWM_CHAN_A, newwrap / 2);
}

int main() {
    // Initialize PWM on buzzer pin
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);

    // Take clockspeed 125 MHz, divide by 125 = 1MHz
    pwm_set_clkdiv(slice, 125.0f);
    // 1MHz / 1000 = 1kHz
    set_tone(slice, 1000);
    // Enable PWM
    pwm_set_enabled(slice, true);

    while (true) {
        set_tone(slice, 1000); // 1kHz
        sleep_ms(500);
        set_tone(slice, 500); // 2kHz
        sleep_ms(500);
    }
}
