#include "game.h"

int main() {
    initialize_joystick();
    LED_Info info = initialize_led_panel();

    State state = STATE_MENU;

    while (true) {
        switch (state) {
        case STATE_MENU:
            state = menu_state(info);
            break;

        case STATE_PLAYING:
            state = game_state(info);
            break;

        case STATE_GAME_OVER:
            state = game_over_state(info);
            break;
        }
    }

    deinitialize_led_panel(info);
}
