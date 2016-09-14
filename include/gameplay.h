/*
 * gameplay.h
 *
 *  Created on: Aug 30, 2011
 *      Author: nick
 */

#include "list.h"
#include "bitmap.h"
#include "level.h"

#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

struct snake_piece_s {
    int x;
    int y;
    int r;
};

struct snake_t {
    int x;
    int y;
    int r; // rotation 0-north 1-east 2-south 3-west
    int l; // length
    int m; // max
    list_t *body;
    int score;
    int dead;
};

struct tiles_info_s {
    int tile_size;
};

struct video_settings_t {
    // size of the window
    int render_width;
    int render_height;
    // game render space
    int width;
    int height;
    char vsync;
    int fullscreen;
};

struct board_state_t {
    float r_time;
    float r_time_max;

    int r;
};

struct gameplay_state_t {
    int paused;
    int rotating;

    level_t *level;
    struct board_state_t board;
    struct tiles_info_s tiles_info;

    struct snake_t snake_1;

    bitmap_t bitmap_tiles;
    bitmap_t bitmap_snake;

    struct video_settings_t *video_settings;
};

void
gameplay_init(void);

void
gameplay_cleanup(void);

void
gameplay_pause(void);

void
gameplay_resume(void);

void
gameplay_input(void);

void
gameplay_update(double);

void
gameplay_draw(double);

void
gameboard_draw(double);

void
snake_add(void);

void
snakebody_destroy(list_t *);

void
snake_draw(struct snake_t *);

void
snake_update(struct snake_t *, double);

void
snake_turn(struct snake_t *, int);

int
snakebody_bitmask(list_t *);

extern struct gameplay_state_t gameplay_state;

#endif /* GAMEPLAY_H_ */
