#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "led_matrix.h"

void snake(PIO pio, uint sm, uint32_t *grid);
void spiral(PIO pio, uint sm, uint32_t *grid);
void raindrops(PIO pio, uint sm, uint32_t *grid);
void ripple(PIO pio, uint sm, uint32_t *grid);
void random_fill(PIO pio, uint sm, uint32_t *grid);
void rainbow_ripple(PIO pio, uint sm, uint32_t *grid);

#endif
