/*
 * bitmap.c
 *
 *  Created on: 04/06/2011
 *      Author: nick
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"

#include "util.h"
#include "bitmap.h"

void *font = NULL;

bitmap_t *
bitmap_from_file(bitmap_t *bitmap, const char *filename, const char *shortname) {
    if (!file_exists(filename)) {
        file_error_message("file not found", filename);
        return NULL;
    }

    ALLEGRO_BITMAP *new_data = al_load_bitmap(filename);
    if (!new_data) {
        file_error_message("failed to load image", filename);
        return NULL;
    }

    bitmap->data = new_data;
    bitmap->filename = filename;
    bitmap->shortname = shortname;
    bitmap->w = al_get_bitmap_width(new_data);
    bitmap->h = al_get_bitmap_height(new_data);

    return bitmap;
}

bitmap_t *
bitmap_create(bitmap_t *bitmap, int width, int height) {
    bitmap->data = al_create_bitmap(width, height);
    bitmap->w = width;
    bitmap->h = height;
    return bitmap;
}

void
bitmap_draw(bitmap_t *bitmap, int x, int y) {
    al_draw_bitmap((ALLEGRO_BITMAP*)bitmap->data, x, y, 0);
}

void
bitmap_draw_region(bitmap_t *bitmap, int sx, int sy, int sw, int sh, int dx,
                   int dy, int flags) {
    al_draw_bitmap_region((ALLEGRO_BITMAP*)bitmap->data, sx, sy, sw, sh, dx, dy, flags);
}

void
bitmap_draw_rotated(bitmap_t *bitmap, int cx, int cy, int dx, int dy,
                    float angle, int flags) {
    al_draw_rotated_bitmap((ALLEGRO_BITMAP*)bitmap->data, cx, cy, dx, dy, angle, flags);
}

pixel_t
bitmap_pixel(bitmap_t *bitmap, unsigned x, unsigned y) {
    pixel_t p;
    ALLEGRO_BITMAP *al_bmp = (ALLEGRO_BITMAP*)bitmap->data;

    p.r = (unsigned char)al_get_pixel(al_bmp, x, y).r;
    p.g = (unsigned char)al_get_pixel(al_bmp, x, y).g;
    p.b = (unsigned char)al_get_pixel(al_bmp, x, y).b;
    p.a = (unsigned char)al_get_pixel(al_bmp, x, y).a;

    return p;
}

/* frees only the allegro bitmap
 * manually free the bitmap container yourself
 */
void
bitmap_cleanup(bitmap_t *bitmap) {
    al_destroy_bitmap((ALLEGRO_BITMAP*)bitmap->data);
}

bitmap_t *
bitmap_get_target(bitmap_t *bitmap) {
    bitmap->data = al_get_target_bitmap();
    bitmap->w = al_get_bitmap_width((ALLEGRO_BITMAP*)bitmap->data);
    bitmap->h = al_get_bitmap_height((ALLEGRO_BITMAP*)bitmap->data);
    return bitmap;
}

void
bitmap_set_target(bitmap_t *bitmap) {
    al_set_target_bitmap((ALLEGRO_BITMAP *) bitmap->data);
}

void
bitmap_draw_textf(pixel_t color, int x, int y, int flags, const char *string, ...) {
    va_list argptr;
    char buffer[80];

    va_start(argptr, string);
    vsprintf(buffer, string, argptr);
    va_end(argptr);

    al_draw_text((ALLEGRO_FONT *)font, al_map_rgba(0, 0, 0, color.a), x, y + 1, flags,
                 buffer);
    al_draw_text((ALLEGRO_FONT *)font, al_map_rgba(color.r, color.g, color.b, color.a), x, y, flags,
                 buffer);
}
