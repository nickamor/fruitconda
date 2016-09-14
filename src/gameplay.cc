/*
 * gameplay.c
 *
 *  Created on: Aug 30, 2011
 *      Author: nick
 */

#include "gameplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "state.h"
#include "util.h"
#include "menu.h"
#include "input.h"

#define GAME_PI 3.141592

char *pausemenu_items[2] =
        {"Retry", "Quit"};

struct menu_s pause_menu =
        {2, 0, "Paused", pausemenu_items};

gameplay_state_t gameplay_state;

int
collide(struct snake_t *snake) {
    int collide_wall = 0;
    int collide_snake = 0;
    collide_wall = level_tile(gameplay_state.level, snake->x, snake->y);

    list_t *iter = snake->body;
    while (iter) {
        struct snake_piece_s *iter_p = (snake_piece_s *)iter->data;
        if (snake->x == iter_p->x && snake->y == iter_p->y) {
            collide_snake = 1;
            break;
        }
        iter = iter->next;
    }

    return collide_wall || collide_snake;
}

void
gameplay_init() {
    // initialise gameplay_state
    gameplay_state.paused = 0;
    gameplay_state.rotating = 0;

    if (gameplay_state.level) {
        gameplay_state.snake_1.x = gameplay_state.level->w / 2;
        gameplay_state.snake_1.y = gameplay_state.level->h / 2;
    }
    gameplay_state.snake_1.dead = 0;
    gameplay_state.snake_1.r = 0;
    gameplay_state.snake_1.l = 0;
    gameplay_state.snake_1.m = 5;
    gameplay_state.snake_1.score = 0;

    gameplay_state.tiles_info.tile_size = 16;

    memset(&gameplay_state.bitmap_tiles, 0, sizeof(bitmap_t));
    memset(&gameplay_state.bitmap_snake, 0, sizeof(bitmap_t));
    bitmap_from_file(&gameplay_state.bitmap_tiles, "assets/graphics/tileset1.png",
                     "tileset1");
    bitmap_from_file(&gameplay_state.bitmap_snake, "assets/graphics/snake.png",
                     "snake");

    gameplay_state.board.r_time = 0.0;
    gameplay_state.board.r_time_max = 0.25;
    gameplay_state.board.r = 0;
}

void
gameplay_cleanup() {
    snakebody_destroy(gameplay_state.snake_1.body);
    gameplay_state.snake_1.body = NULL;

    bitmap_cleanup(&gameplay_state.bitmap_tiles);
    bitmap_cleanup(&gameplay_state.bitmap_snake);
}

void
gameplay_pause() {
    gameplay_state.paused = 1;
}

void
gameplay_resume() {
    gameplay_state.paused = 0;
}

void
pausemenu_input() {
    if (input_state.up) {
        menu_change_selection(&pause_menu, -1);
    } else if (input_state.down) {
        menu_change_selection(&pause_menu, 1);
    } else if (input_state.button_action) {
        switch (pause_menu.selection) {
            case 1:
                change_state(mainmenu);
                break;
            default:
                change_state(gameplay);
                break;
        }
    } else if (input_state.button_back) {
        gameplay_state.paused = 0;
    }
}

void
gameplay_input() {
    if (input_state.button_pause) {
        if (!gameplay_state.paused) {
            game_state.state_pause();
        } else {
            game_state.state_resume();
        }
        return;
    }

    if (gameplay_state.paused) {
        pausemenu_input();
    } else {
        if (gameplay_state.snake_1.dead)
            return;
        if (input_state.left && !gameplay_state.rotating) {
            snake_turn(&gameplay_state.snake_1, 1);
        } else if (input_state.right && !gameplay_state.rotating) {
            snake_turn(&gameplay_state.snake_1, -1);
        } else if (input_state.button_back) {
            change_state(nullstate);
        }
    }
}

double snake_dead_anim = 0.0, snake_dead_anim_max = 2.0;

void
gameplay_update(double delta_time) {
    if (!(gameplay_state.paused | gameplay_state.rotating)) {
        if (!gameplay_state.snake_1.dead) {
            snake_update(&gameplay_state.snake_1, delta_time);
        } else {
            if (snake_dead_anim < snake_dead_anim_max) {
                snake_dead_anim += delta_time;
            } else {
                snake_dead_anim = 0.0;
                change_state(gameplay);
            }
        }
    }
}

void
pausemenu_draw() {
    menu_draw(&pause_menu, 50, 50);
}

/**
 * Decides what should be drawn
 */
void
gameplay_draw(double delta_time) {
    if (gameplay_state.level) {
        gameboard_draw(delta_time);
    }

    if (gameplay_state.paused) {
        pausemenu_draw();
    }
}

/** void draw_gameboard()
 * draw the current level
 */
void
gameboard_draw(double delta_time) {
    int ts = gameplay_state.tiles_info.tile_size;
    struct board_state_t *b = &gameplay_state.board;
    int level_width = gameplay_state.level->w;
    int level_height = gameplay_state.level->h;

    bitmap_t buffer_board;
    bitmap_t buffer;

    bitmap_create(&buffer_board, ts * level_width, ts * level_height);
    bitmap_get_target(&buffer);
    bitmap_set_target(&buffer_board);

    level_draw(gameplay_state.level, 0, 0);

    snake_draw(&gameplay_state.snake_1);

    bitmap_set_target(&buffer);

    /* rotate the board */
    double r = (GAME_PI / 2) * b->r;

    int snake_delta_r = gameplay_state.snake_1.r - b->r;
    if (snake_delta_r > 1) {
        snake_delta_r = -1;
    } else if (snake_delta_r < -1) {
        snake_delta_r = 1;
    }

    if (snake_delta_r != 0) {
        gameplay_state.rotating = 1;
        b->r_time += delta_time;
        double r_max = (GAME_PI / 2) * snake_delta_r;
        double r_degree = (-cos(b->r_time / b->r_time_max) + 1) * 2;
        double delta_r = r_max * r_degree;

        r += delta_r;

        if (b->r_time > b->r_time_max) {
            b->r_time = 0.0;
            b->r = gameplay_state.snake_1.r;
        }
    } else {
        gameplay_state.rotating = 0;
    }

// draw rotated board - cx, cy: the center of the board bitmap
// bx, by: where to draw the board
    int cx = buffer_board.w / 2;
    int cy = buffer_board.h / 2;
    int bx = gameplay_state.video_settings->width / 2;
    int by = gameplay_state.video_settings->height / 2;
    bitmap_draw_rotated(&buffer_board, cx, cy, bx, by, r, 0);

    bitmap_cleanup(&buffer_board);
}

/** void snake_add()
 * updates the snake body position
 */
void
snake_add() {
    struct snake_piece_s* new_snake = (snake_piece_s *)safe_malloc(sizeof(struct snake_piece_s));
    new_snake->x = gameplay_state.snake_1.x;
    new_snake->y = gameplay_state.snake_1.y;
    new_snake->r = gameplay_state.snake_1.r;

    gameplay_state.snake_1.body = list_push(gameplay_state.snake_1.body,
                                            (void *) new_snake);
    ++gameplay_state.snake_1.l;

    // enforce maximum length
    while (gameplay_state.snake_1.l >= gameplay_state.snake_1.m) {
        list_t *tail = list_foot(gameplay_state.snake_1.body);

        // ... by chopping off any extra bits
        tail->prev->next = NULL;
        free(tail->data);
        free(tail);

        --gameplay_state.snake_1.l;
    }
}

/** void snake_destroy()
 * free the snake-body-occupied memory
 */
void
snakebody_destroy(list_t *snakebody) {
    list_t *iter = snakebody;
    while (iter) {
        list_t *next_iter = iter->next;
        if (iter->data) {
            free(iter->data);
        }
        free(iter);
        iter->next = NULL;
        iter = next_iter;
    }
}

void
snake_draw(struct snake_t *snake) {
    int ts = gameplay_state.tiles_info.tile_size;
    bitmap_t *snake_gfx = &gameplay_state.bitmap_snake;

    // dead blink animation
    if (snake->dead) {
        if (((int) (snake_dead_anim * 8) % 2) == 0) {
            return;
        }
    }

    if (gameplay_state.snake_1.body) {
        int c_up = 0, c_down = 0, c_left = 0, c_right = 0;
        int nx, ny;

        nx = ((struct snake_piece_s *) gameplay_state.snake_1.body->data)->x;
        ny = ((struct snake_piece_s *) gameplay_state.snake_1.body->data)->y;

        // handle looped parts
        if (nx == 0 && snake->x == gameplay_state.level->w - 1) {
            nx = snake->x + 1;
        }
        if (ny == 0 && snake->y == gameplay_state.level->h - 1) {
            ny = snake->y + 1;
        }
        if (nx == gameplay_state.level->w - 1 && snake->x == 0) {
            nx = snake->x - 1;
        }
        if (ny == gameplay_state.level->h - 1 && snake->y == 0) {
            ny = snake->y - 1;
        }

        // which way is the next piece
        if (nx < snake->x) {
            c_left = 1;
        }
        if (nx > snake->x) {
            c_right = 1;
        }
        if (ny < snake->y) {
            c_up = 1;
        }
        if (ny > snake->y) {
            c_down = 1;
        }

        // draw snake head
        if (snake->r == 0) {
            if (c_down) {
                bitmap_draw_region(snake_gfx, 0, 0, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_left) {
                bitmap_draw_region(snake_gfx, 0, 16, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_right) {
                bitmap_draw_region(snake_gfx, 0, 32, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
        }
        if (snake->r == 1) {
            if (c_right) {
                bitmap_draw_region(snake_gfx, 16, 0, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_down) {
                bitmap_draw_region(snake_gfx, 16, 16, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_up) {
                bitmap_draw_region(snake_gfx, 16, 32, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
        }
        if (snake->r == 2) {
            if (c_up) {
                bitmap_draw_region(snake_gfx, 32, 0, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_left) {
                bitmap_draw_region(snake_gfx, 32, 16, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_right) {
                bitmap_draw_region(snake_gfx, 32, 32, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
        }
        if (snake->r == 3) {
            if (c_left) {
                bitmap_draw_region(snake_gfx, 48, 0, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_up) {
                bitmap_draw_region(snake_gfx, 48, 16, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
            if (c_down) {
                bitmap_draw_region(snake_gfx, 48, 32, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
            }
        }
    } else {
        switch (snake->r) {
            case 0:
                bitmap_draw_region(snake_gfx, 0, 48, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
                break;
            case 1:
                bitmap_draw_region(snake_gfx, 16, 48, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
                break;
            case 2:
                bitmap_draw_region(snake_gfx, 32, 48, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
                break;
            case 3:
                bitmap_draw_region(snake_gfx, 48, 48, ts, ts, snake->x * ts,
                                   snake->y * ts, 0);
                break;
        }
    }

// draw snake body
    list_t *iter;
    iter = snake->body;
    while (iter) {
        int sx = ((struct snake_piece_s *) iter->data)->x;
        int sy = ((struct snake_piece_s *) iter->data)->y;

        int c = snakebody_bitmask(iter);

        switch (c) {
            case 1: // Bottom End
                bitmap_draw_region(snake_gfx, 0, 64, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 2: // Right End
                bitmap_draw_region(snake_gfx, 16, 64, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 3: // Bottom-Right
                bitmap_draw_region(snake_gfx, 32, 64, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 4: // Left End
                bitmap_draw_region(snake_gfx, 48, 64, ts, ts, sx * ts, sy * ts, 0);
                break;

            case 5: // Bottom-Left
                bitmap_draw_region(snake_gfx, 0, 80, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 6: // Left-Right
                bitmap_draw_region(snake_gfx, 16, 80, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 8: // Top End
                bitmap_draw_region(snake_gfx, 32, 80, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 9: // Top-Down
                bitmap_draw_region(snake_gfx, 48, 80, ts, ts, sx * ts, sy * ts, 0);
                break;

            case 10: // Top-Right
                bitmap_draw_region(snake_gfx, 0, 96, ts, ts, sx * ts, sy * ts, 0);
                break;
            case 12: // Top-Left
                bitmap_draw_region(snake_gfx, 16, 96, ts, ts, sx * ts, sy * ts, 0);
                break;
        }

        iter = iter->next;
    }
}

/* for a given snake-body-part, how do we draw it? */
enum snakebody_bit {
    bit_up = 8, bit_left = 4, bit_right = 2, bit_down = 1
};

int
snakebody_bitmask(list_t *bodypart) {
    int up = 0, down = 0, left = 0, right = 0;
    int bitmask = 0;
    int this_x, this_y, prev_x, prev_y, next_x, next_y;

    this_x = ((struct snake_piece_s *) bodypart->data)->x;
    this_y = ((struct snake_piece_s *) bodypart->data)->y;

// if this bodypart is the first body part, the prev is actually the head
    if (bodypart->prev != NULL) {
        prev_x = ((struct snake_piece_s *) bodypart->prev->data)->x;
        prev_y = ((struct snake_piece_s *) bodypart->prev->data)->y;
    } else {
        prev_x = gameplay_state.snake_1.x;
        prev_y = gameplay_state.snake_1.y;
    }

// handle bodyparts on the other side of the board due to looping
    if (this_x == 0 && prev_x == gameplay_state.level->w - 1) {
        prev_x = this_x - 1;
    }
    if (this_y == 0 && prev_y == gameplay_state.level->h - 1) {
        prev_y = this_y - 1;
    }
    if (this_x == gameplay_state.level->w - 1 && prev_x == 0) {
        prev_x = this_x + 1;
    }
    if (this_y == gameplay_state.level->h - 1 && prev_y == 0) {
        prev_y = this_y + 1;
    }

// add prev to bitmask
    if (prev_y < this_y) {
        up = 1;
    }
    if (prev_y > this_y) {
        down = 1;
    }
    if (prev_x < this_x) {
        left = 1;
    }
    if (prev_x > this_x) {
        right = 1;
    }

    if (bodypart->next) {
        next_x = ((struct snake_piece_s *) bodypart->next->data)->x;
        next_y = ((struct snake_piece_s *) bodypart->next->data)->y;

        // handle bodyparts on the other side of the board due to looping
        if (this_x == 0 && next_x == gameplay_state.level->w - 1) {
            next_x = this_x - 1;
        }
        if (this_y == 0 && next_y == gameplay_state.level->h - 1) {
            next_y = this_y - 1;
        }
        if (this_x == gameplay_state.level->w - 1 && next_x == 0) {
            next_x = this_x + 1;
        }
        if (this_y == gameplay_state.level->h - 1 && next_y == 0) {
            next_y = this_y + 1;
        }

        // add next to bitmask
        if (next_y < this_y) {
            up = 1;
        }
        if (next_y > this_y) {
            down = 1;
        }
        if (next_x < this_x) {
            left = 1;
        }
        if (next_x > this_x) {
            right = 1;
        }
    }

    bitmask = (up * bit_up) + (left * bit_left) + (right * bit_right)
              + (down * bit_down);

    return bitmask;
}

/*
 *  move the snake
 */
double snake_update_primer = 0.0, snake_update_max = 1.0;
int difficulty = 2;

void
snake_update(struct snake_t *snake, double delta_time) {
    /* only move snake every some many updates */
    snake_update_primer += delta_time;
    if (snake_update_primer < snake_update_max / difficulty) {
        return;
    } else {
        snake_update_primer = 0;
    }

    snake_add();
    switch (snake->r) {
        case 0:
            snake->y -= 1;
            break;
        case 1:
            snake->x -= 1;
            break;
        case 2:
            snake->y += 1;
            break;
        case 3:
            snake->x += 1;
            break;
    }

// snake wrap
    if (snake->x >= gameplay_state.level->w) {
        snake->x = 0;
    }
    if (snake->y >= gameplay_state.level->h) {
        snake->y = 0;
    }
    if (snake->x < 0) {
        snake->x = gameplay_state.level->w - 1;
    }
    if (snake->y < 0) {
        snake->y = gameplay_state.level->h - 1;
    }

    if (collide(snake)) {
        snake->dead = 1;
    }
}

void
snake_turn(struct snake_t *snake, int r) {
    int temp = snake->r;
    temp += r;

    // prevent from rotating backwards onto yourself
    if (snake->body) {
        int prev_r = ((struct snake_piece_s *) snake->body->data)->r;
        if (temp != prev_r + 2 && temp != prev_r - 2) {
            // wrap
            if (temp > 3) {
                temp = 0;
            }
            if (temp < 0) {
                temp = 3;
            }

            snake->r = temp;
        }
    } else {

        snake->r = temp;
    }

    gameplay_state.rotating = 1;
}

