#include "game.h"
#include "pico/stdlib.h"

State menu_state(LED_Info info) {
    while (!read_button()) {
        sleep_ms(10);
    }
    while (read_button()) {
        sleep_ms(10);
    }
    return STATE_PLAYING;
}

State game_state(LED_Info info) {
    // TODO
    return STATE_GAME_OVER;
}

State game_over_state(LED_Info info) {
    // TODO
    return STATE_MENU;
}
