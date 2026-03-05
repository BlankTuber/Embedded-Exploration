#include "game.h"
#include "pico/stdlib.h"
#include <stdlib.h>

State menu_state(LED_Info info) {
    uint32_t grid[NUM_PIXELS];

    int b1x = 4, b1y = 4, d1x = 1, d1y = 1;
    int b2x = 12, b2y = 11, d2x = -1, d2y = 1;
    int left_y = 6, right_y = 6;
    const int paddle_size = 4;

    int t1x[TAIL_LEN], t1y[TAIL_LEN];
    int t2x[TAIL_LEN], t2y[TAIL_LEN];
    for (int i = 0; i < TAIL_LEN; i++) {
        t1x[i] = b1x;
        t1y[i] = b1y;
        t2x[i] = b2x;
        t2y[i] = b2y;
    }

    int hue = 0;

    while (!read_button()) {
        // Shift tails
        for (int i = TAIL_LEN - 1; i > 0; i--) {
            t1x[i] = t1x[i - 1];
            t1y[i] = t1y[i - 1];
            t2x[i] = t2x[i - 1];
            t2y[i] = t2y[i - 1];
        }
        t1x[0] = b1x;
        t1y[0] = b1y;
        t2x[0] = b2x;
        t2y[0] = b2y;

        // Move balls
        b1x += d1x;
        b1y += d1y;
        if (b1x <= 0 || b1x >= MATRIX_SIDE - 1)
            d1x = -d1x;
        if (b1y <= 0 || b1y >= MATRIX_SIDE - 1)
            d1y = -d1y;

        b2x += d2x;
        b2y += d2y;
        if (b2x <= 0 || b2x >= MATRIX_SIDE - 1)
            d2x = -d2x;
        if (b2y <= 0 || b2y >= MATRIX_SIDE - 1)
            d2y = -d2y;

        // Left paddle tracks whichever ball is closer to it
        int lc = left_y + paddle_size / 2;
        int tl = (b1x <= b2x) ? b1y : b2y;
        if (lc < tl && left_y < MATRIX_SIDE - paddle_size)
            left_y++;
        else if (lc > tl && left_y > 0)
            left_y--;

        // Right paddle tracks whichever ball is closer to it
        int rc = right_y + paddle_size / 2;
        int tr = (b2x >= b1x) ? b2y : b1y;
        if (rc < tr && right_y < MATRIX_SIDE - paddle_size)
            right_y++;
        else if (rc > tr && right_y > 0)
            right_y--;

        hue = (hue + 3) % 360;

        clear_grid(grid);

        // Tails: hue shifts along length, ball 2 is opposite hue
        for (int i = 0; i < TAIL_LEN; i++) {
            uint8_t brightness = 0x20 >> i;
            grid[xy(t1x[i], t1y[i])] =
                hsv_to_rgb((hue + i * 15) % 360, brightness);
            grid[xy(t2x[i], t2y[i])] =
                hsv_to_rgb((hue + 180 + i * 15) % 360, brightness);
        }

        // Paddles with a slow rainbow gradient
        for (int i = 0; i < paddle_size; i++) {
            int ph = (hue + i * 25) % 360;
            grid[xy(0, left_y + i)] = hsv_to_rgb(ph, 0x08);
            grid[xy(MATRIX_SIDE - 1, right_y + i)] =
                hsv_to_rgb((ph + 180) % 360, 0x08);
        }

        // Balls on top
        grid[xy(b1x, b1y)] = hsv_to_rgb(hue, 0x20);
        grid[xy(b2x, b2y)] = hsv_to_rgb((hue + 180) % 360, 0x20);

        put_pixels(info.pio, info.sm, grid);
        sleep_ms(80);
    }

    while (read_button())
        sleep_ms(10);
    return STATE_PLAYING;
}

// Top half deflects up, bottom half deflects down.
// Ghost zones at ±1 beyond paddle edges always deflect outward.
static bool check_paddle_collision(int ball_y, int paddle_y, int paddle_size,
                                   int *out_dy) {
    if (ball_y == paddle_y - 1) {
        *out_dy = -1;
        return true;
    }
    if (ball_y == paddle_y + paddle_size) {
        *out_dy = 1;
        return true;
    }
    if (ball_y >= paddle_y && ball_y < paddle_y + paddle_size) {
        *out_dy = (ball_y < paddle_y + paddle_size / 2) ? -1 : 1;
        return true;
    }
    return false;
}

State game_state(LED_Info info) {
    uint32_t grid[NUM_PIXELS] = {0};

    GameState game = {.ball_x = 8,
                      .ball_y = 8,
                      .ball_dx = 1,
                      .ball_dy = 1,
                      .ball_speed = 5,
                      .hit_count = 0,
                      .player_y = 6,
                      .ai_y = 6,
                      .player_score = 0,
                      .ai_score = 0,
                      .paddle_size = 4};

    int ball_counter = 0;
    int ai_counter = 0;
    int ai_speed = 1;

    while (true) {
        // Input
        int input = read_adc_y();
        game.player_y += input;
        if (game.player_y < 0)
            game.player_y = 0;
        if (game.player_y > MATRIX_SIDE - game.paddle_size)
            game.player_y = MATRIX_SIDE - game.paddle_size;

        // Update ball
        ball_counter++;
        if (ball_counter >= game.ball_speed) {
            ball_counter = 0;
            game.ball_x += game.ball_dx;
            game.ball_y += game.ball_dy;

            if (game.ball_y <= 0) {
                game.ball_y = 0;
                game.ball_dy = -game.ball_dy;
            }
            if (game.ball_y >= MATRIX_SIDE - 1) {
                game.ball_y = MATRIX_SIDE - 1;
                game.ball_dy = -game.ball_dy;
            }

            // Player collision
            if (game.ball_x == 1) {
                int new_dy = game.ball_dy;
                if (check_paddle_collision(game.ball_y, game.player_y,
                                           game.paddle_size, &new_dy)) {
                    game.ball_dx = 1;
                    game.ball_dy = new_dy;
                    game.hit_count++;
                    if (game.hit_count % 5 == 0) {
                        if (game.ball_speed > 1)
                            game.ball_speed--;
                        else if (game.paddle_size > 2)
                            game.paddle_size--;
                    }
                }
            }

            // AI collision
            if (game.ball_x == 14) {
                int new_dy = game.ball_dy;
                if (check_paddle_collision(game.ball_y, game.ai_y,
                                           game.paddle_size, &new_dy)) {
                    game.ball_dx = -1;
                    game.ball_dy = new_dy;
                    game.hit_count++;
                    if (game.hit_count % 5 == 0) {
                        if (game.ball_speed > 1)
                            game.ball_speed--;
                        else if (game.paddle_size > 2)
                            game.paddle_size--;
                    }
                }
            }

            if (game.ball_x <= 0) {
                game.ai_score++;
                return STATE_GAME_OVER_AI;
            }
            if (game.ball_x >= MATRIX_SIDE - 1) {
                game.player_score++;
                return STATE_GAME_OVER_PLAYER;
            }
        }

        // AI tracking + weighted corner redirect
        ai_counter++;
        if (ai_counter >= ai_speed) {
            ai_counter = 0;
            int ai_center = game.ai_y + game.paddle_size / 2;

            if (ai_center < game.ball_y)
                game.ai_y++;
            else if (ai_center > game.ball_y)
                game.ai_y--;
            else {
                // 40% top half, 40% bottom half, 20% stay
                int r = rand() % 10;
                if (r < 4) {
                    if (game.ai_y < game.ball_y)
                        game.ai_y++;
                } else if (r < 8) {
                    int target = game.ball_y - game.paddle_size + 1;
                    if (game.ai_y > target)
                        game.ai_y--;
                }
            }

            if (game.ai_y < 0)
                game.ai_y = 0;
            if (game.ai_y > MATRIX_SIDE - game.paddle_size)
                game.ai_y = MATRIX_SIDE - game.paddle_size;
        }

        // Render
        clear_grid(grid);

        for (int i = 0; i < game.paddle_size; i++) {
            grid[xy(0, game.player_y + i)] = GREEN;
            grid[xy(15, game.ai_y + i)] = RED;
        }
        grid[xy(game.ball_x, game.ball_y)] = BLUE;

        put_pixels(info.pio, info.sm, grid);
        sleep_ms(FRAME_DELAY_MS);
    }
}

State game_over_state(LED_Info info, Winner winner) {
    uint32_t grid[NUM_PIXELS];
    uint32_t color = (winner == WINNER_AI) ? RED : GREEN;

    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0)
            for (int p = 0; p < NUM_PIXELS; p++)
                grid[p] = color;
        else
            clear_grid(grid);
        put_pixels(info.pio, info.sm, grid);
        sleep_ms(300);
    }

    return STATE_MENU;
}
