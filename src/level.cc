/*
 * level.c
 *
 *  Created on: Aug 18, 2011
 *      Author: nick
 */

#include <stdio.h>
#include <stdlib.h>
#include "gameplay.h"
#include "util.h"

#define DEFAULT_LEVEL_W 12
#define DEFAULT_LEVEL_H 12
unsigned char default_level[DEFAULT_LEVEL_W * DEFAULT_LEVEL_H] =
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, //
         1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, //
         1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, //
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //
         1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, //
         1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, //
         1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1};

/** level level_from_default()
 * create a new level using the default layout
 */
void
level_load_default(level_t *level) {
    level->w = DEFAULT_LEVEL_W;
    level->h = DEFAULT_LEVEL_H;
    level->tiles = default_level;
}

void
level_load_fromfile(level_t *level, const char *filename) {
    bitmap_t level_bmp;
    bitmap_from_file(&level_bmp, filename, "level_file");

    level->w = level_bmp.w;
    level->h = level_bmp.h;

    /* allocate tiles array */
    size_t tiles_array_size = (sizeof(unsigned char) * (level->w * level->h));
    level->tiles = (unsigned char *)safe_malloc(tiles_array_size);

    int y = 0;
    while (y < level->h) {
        int x = 0;
        while (x < level->w) {
            pixel_t p = bitmap_pixel(&level_bmp, x, y);

            int tile = (p.r && p.g && p.b) ? 1 : 0;

            level->tiles[x + (y * level->w)] = tile;
            ++x;
        }
        ++y;
    }

    bitmap_cleanup(&level_bmp);
}

/**
 * returns the tile type at the specified level position
 * accepts coordinates outside the level, ie. wraps around appropriately
 */
int
level_tile(level_t *level, int x, int y) {
    while (x > level->w - 1) {
        x -= level->w;
    }
    while (x < 0) {
        x += level->w;
    }
    while (y > level->h - 1) {
        y -= level->h;
    }
    while (y < 0) {
        y += level->h;
    }

    return level->tiles[x + (y * level->w)];
}

/**
 *  returns a bitmask specifying whether a tile is surround by similar tiles
 */
int
level_tile_bitmask(level_t *level, int x, int y) {
    enum level_tile_bitmask_e {
        up = 1 << 3, right = 1 << 2, down = 1 << 1, left = 1
    };
    int bitmask = 0;
    int tile_type = level_tile(level, x, y);

    if (level_tile(level, x, y - 1) == tile_type) {
        bitmask += up;
    }
    if (level_tile(level, x, y + 1) == tile_type) {
        bitmask += down;
    }
    if (level_tile(level, x - 1, y) == tile_type) {
        bitmask += left;
    }
    if (level_tile(level, x + 1, y) == tile_type) {
        bitmask += right;
    }

    return bitmask;
}

void
level_draw(level_t *level, int dx, int dy) {
    int ts = gameplay_state.tiles_info.tile_size;

    int y = 0;
    while (y < level->h) {
        int x = 0;
        while (x < level->w) {
            level_tile_draw(level, x, y, dx + (x * ts), dy + (y * ts));
            ++x;
        }
        ++y;
    }
}

void
level_tile_draw(level_t *level, int x, int y, int dx, int dy) {
    bitmap_t *tiles = &gameplay_state.bitmap_tiles;
    int ts = gameplay_state.tiles_info.tile_size;

    bitmap_draw_region(tiles, 0, 0, ts, ts, dx, dy, 0);

    /* TODO use ts variables instead of constants */
    if (level_tile(level, x, y) == 1) {
        switch (level_tile_bitmask(level, x, y)) {
            case 15:
                bitmap_draw_region(tiles, 48, 48, 16, 16, dx + 0, dx + 0, 0);
                break;
            case 14:
                bitmap_draw_region(tiles, 0, 32, 8, 16, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 56, 48, 8, 16, dx + 8, dy + 0, 0);
                break;
            case 13:
                bitmap_draw_region(tiles, 48, 48, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 16, 56, 16, 8, dx + 0, dy + 8, 0);
                break;
            case 12:
                bitmap_draw_region(tiles, 0, 48, 8, 16, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 56, 48, 8, 8, dx + 8, dy + 0, 0);
                bitmap_draw_region(tiles, 8, 56, 8, 8, dx + 8, dy + 8, 0);
                break;
            case 11:
                bitmap_draw_region(tiles, 48, 48, 8, 16, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 40, 32, 8, 16, dx + 8, dy + 0, 0);
                break;
            case 10:
                bitmap_draw_region(tiles, 0, 32, 8, 16, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 40, 32, 8, 16, dx + 8, dy + 0, 0);
                break;
            case 9:
                bitmap_draw_region(tiles, 48, 48, 8, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 32, 56, 8, 8, dx + 0, dy + 8, 0);
                bitmap_draw_region(tiles, 40, 48, 8, 16, dx + 8, dy + 0, 0);
                break;
            case 8:
                bitmap_draw_region(tiles, 0, 48, 8, 16, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 40, 48, 8, 16, dx + 8, dy + 0, 0);
                break;
            case 7:
                bitmap_draw_region(tiles, 16, 16, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 48, 56, 16, 8, dx + 0, dy + 8, 0);
                break;
            case 6:
                bitmap_draw_region(tiles, 0, 16, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 0, 24, 8, 8, dx + 0, dy + 8, 0);
                bitmap_draw_region(tiles, 56, 56, 8, 8, dx + 8, dy + 8, 0);
                break;
            case 5:
                bitmap_draw_region(tiles, 16, 16, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 16, 56, 16, 8, dx + 0, dy + 8, 0);
                break;
            case 4:
                bitmap_draw_region(tiles, 0, 16, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 0, 56, 16, 8, dx + 0, dy + 8, 0);
                break;
            case 3:
                bitmap_draw_region(tiles, 32, 16, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 48, 56, 8, 8, dx + 0, dy + 8, 0);
                bitmap_draw_region(tiles, 40, 24, 8, 8, dx + 8, dy + 8, 0);
                break;
            case 2:
                bitmap_draw_region(tiles, 0, 16, 8, 16, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 40, 16, 8, 16, dx + 8, dy + 0, 0);
                break;
            case 1:
                bitmap_draw_region(tiles, 32, 16, 16, 8, dx + 0, dy + 0, 0);
                bitmap_draw_region(tiles, 32, 56, 16, 8, dx + 0, dy + 8, 0);
                break;
            case 0:
                bitmap_draw_region(tiles, 48, 16, 16, 16, dx + 0, dy + 0, 0);
                break;
        }
    }
}

/*
 * free satellite data
 */
void
level_destroy(level_t *level) {
    if (level->tiles != default_level) {
        free(level->tiles);
    }
}
