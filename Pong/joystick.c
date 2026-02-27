#include "joystick.h"

void initialize_joystick() {
    adc_init();
    adc_gpio_init(VSX_PIN);
    adc_gpio_init(VSY_PIN);

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);
}

JoystickDir read_adc_all() {
    adc_select_input(0);
    uint adc_x_raw = adc_read();

    adc_select_input(1);
    uint adc_y_raw = adc_read();

    JoystickDir dir = {0, 0};

    if (adc_x_raw > 2548)
        dir.x = 1;
    else if (adc_x_raw < 1548)
        dir.x = -1;

    if (adc_y_raw > 2548)
        dir.y = 1;
    else if (adc_y_raw < 1548)
        dir.y = -1;

    return dir;
}

int read_adc_x() {
    adc_select_input(0);
    uint adc_x_raw = adc_read();

    if (adc_x_raw > 2548) {
        return 1;
    } else if (adc_x_raw < 1548) {
        return -1;
    } else {
        return 0;
    }
}

int read_adc_y() {
    adc_select_input(1);
    uint adc_y_raw = adc_read();

    if (adc_y_raw > 2548) {
        return 1;
    } else if (adc_y_raw < 1548) {
        return -1;
    } else {
        return 0;
    }
}

bool read_button() { return !gpio_get(SW_PIN); }
