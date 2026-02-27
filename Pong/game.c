#include "game.h"
#include "pico/stdlib.h"

State menu_state(LED_Info info) {
    uint32_t grid[NUM_PIXELS];
    int ball_x = 8, ball_y = 8;
    int dx = 1, dy = 1;

    while (!read_button()) {
        clear_grid(grid);

        ball_x += dx;
        ball_y += dy;

        if (ball_x <= 0 || ball_x >= MATRIX_SIDE - 1)
            dx = -dx;
        if (ball_y <= 0 || ball_y >= MATRIX_SIDE - 1)
            dy = -dy;

        grid[xy(ball_x, ball_y)] = BLUE;

        put_pixels(info.pio, info.sm, grid);
        sleep_ms(80);
    }
    while (read_button())
        sleep_ms(10);

    return STATE_PLAYING;
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
    int ai_speed = 2;

    while (true) {
        // Input
        int input = read_adc_y();
        game.player_y += input;
        if (game.player_y < 0)
            game.player_y = 0;
        if (game.player_y > MATRIX_SIDE - game.paddle_size)
            game.player_y = MATRIX_SIDE - game.paddle_size;

        // Update Ball
        ball_counter++;
        if (ball_counter >= game.ball_speed) {
            ball_counter = 0;
            game.ball_x += game.ball_dx;
            game.ball_y += game.ball_dy;

            // Bounce on top/bottom
            if (game.ball_y <= 0) {
                game.ball_y = 0;
                game.ball_dy = -game.ball_dy;
            }
            if (game.ball_y >= MATRIX_SIDE - 1) {
                game.ball_y = MATRIX_SIDE - 1;
                game.ball_dy = -game.ball_dy;
            }

            // Check player collision
            if (game.ball_x == 1 && game.ball_y >= game.player_y &&
                game.ball_y < game.player_y + game.paddle_size) {
                game.ball_dx = 1;
                int hit_pos = game.ball_y - game.player_y;
                if (hit_pos == 0)
                    game.ball_dy = -1;
                else if (hit_pos == game.paddle_size - 1)
                    game.ball_dy = 1;

                game.hit_count++;
                if (game.hit_count % 5 == 0) {
                    if (game.ball_speed > 1)
                        game.ball_speed--;
                    else if (game.paddle_size > 2)
                        game.paddle_size--;
                }
            }

            // Check AI collision
            if (game.ball_x == 14 && game.ball_y >= game.ai_y &&
                game.ball_y < game.ai_y + game.paddle_size) {
                game.ball_dx = -1;
                int hit_pos = game.ball_y - game.ai_y;
                if (hit_pos == 0)
                    game.ball_dy = -1;
                else if (hit_pos == game.paddle_size - 1)
                    game.ball_dy = 1;

                game.hit_count++;
                if (game.hit_count % 5 == 0) {
                    if (game.ball_speed > 1)
                        game.ball_speed--;
                    else if (game.paddle_size > 2)
                        game.paddle_size--;
                }
            }

            // Score points
            if (game.ball_x <= 0) {
                game.ai_score++;
                return STATE_GAME_OVER_AI;
            }
            if (game.ball_x >= MATRIX_SIDE - 1) {
                game.player_score++;
                return STATE_GAME_OVER_PLAYER;
            }
        }

        // Simple AI
        ai_counter++;
        if (ai_counter >= ai_speed) {
            ai_counter = 0;
            int ai_center = game.ai_y + game.paddle_size / 2;
            if (ai_center < game.ball_y)
                game.ai_y++;
            else if (ai_center > game.ball_y)
                game.ai_y--;
            if (game.ai_y < 0)
                game.ai_y = 0;
            if (game.ai_y > MATRIX_SIDE - game.paddle_size)
                game.ai_y = MATRIX_SIDE - game.paddle_size;
        }

        // Rendering
        clear_grid(grid);

        // Render paddles
        for (int i = 0; i < game.paddle_size; i++) {
            grid[xy(0, game.player_y + i)] = GREEN;
            grid[xy(15, game.ai_y + i)] = RED;
        }

        // Ball
        grid[xy(game.ball_x, game.ball_y)] = BLUE;

        put_pixels(info.pio, info.sm, grid);
        sleep_ms(FRAME_DELAY_MS);
    }
}

State game_over_state(LED_Info info, Winner winner) {
    uint32_t grid[NUM_PIXELS];
    uint32_t color;
    if (winner == WINNER_AI)
        color = RED;
    else
        color = GREEN;

    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0) {
            for (int p = 0; p < NUM_PIXELS; p++) {
                grid[p] = color;
            }
        } else {
            clear_grid(grid);
        }
        put_pixels(info.pio, info.sm, grid);
        sleep_ms(300);
    }

    return STATE_MENU;
}
