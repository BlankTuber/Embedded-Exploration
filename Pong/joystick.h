#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "hardware/adc.h"

#define VSX_PIN 26
#define VSY_PIN 27
#define SW_PIN 22

typedef struct {
    int x;
    int y;
} JoystickDir;

void initialize_joystick();
JoystickDir read_adc_all();
int read_adc_x();
int read_adc_y();
bool read_button();

#endif
