#ifndef GAME_H
#define GAME_H

#include "display.h"
#include "joystick.h"

#define FRAME_DELAY_MS 33

// Colors
#define BLACK 0
#define RED convert_rgb(0x05, 0, 0)
#define GREEN convert_rgb(0, 0x05, 0)
#define BLUE convert_rgb(0, 0, 0x05)

typedef struct {
    int ball_x, ball_y;
    int ball_dx, ball_dy;
    int ball_speed;
    int hit_count;
    int player_y;
    int ai_y;
    int player_score, ai_score;
    int paddle_size;
} GameState;

typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAME_OVER } State;

State menu_state(LED_Info info);
State game_state(LED_Info info);
State game_over_state(LED_Info info);

#endif
