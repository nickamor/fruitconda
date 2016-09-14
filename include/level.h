/*
 * level.h
 *
 *  Created on: Aug 18, 2011
 *      Author: nick
 */

#ifndef LEVEL_H_
#define LEVEL_H_

typedef struct level_s level_t;

struct level_s {
    unsigned char w;
    unsigned char h;
    unsigned char *tiles;
};

enum tile_type {
    empty = 0,
    block = 1
};

void
level_load_default(level_t *);

void
level_load_fromfile(level_t *, const char *);

int
level_tile(level_t *, int, int);

int
level_tile_bitmask(level_t *, int, int);

void
level_draw(level_t *, int, int);

void
level_tile_draw(level_t *, int, int, int, int);

void
level_destroy(level_t *);

#endif /* LEVEL_H_ */
