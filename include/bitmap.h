/*
 * bitmap.h
 *
 *  Created on: 04/06/2011
 *      Author: nick
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "list.h"

typedef struct bitmap_s bitmap_t;

struct bitmap_s {
    const char *filename;
    const char *shortname;
    int w;
    int h;
    void *data;
};

struct sprite_s {
    bitmap_t *bitmap;
    /* where in the bitmap does this sprite start? */
    unsigned char x;
    unsigned char y;
    unsigned char w;
    unsigned char h;
};

typedef struct pixel_s pixel_t;

struct pixel_s {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

bitmap_t *
bitmap_from_file(bitmap_t *, const char *, const char *);

bitmap_t *
bitmap_from_databitmap(bitmap_t *, void *, const char *);

bitmap_t *
bitmap_create(bitmap_t *, int, int);

pixel_t
bitmap_pixel(bitmap_t *, unsigned, unsigned);

extern list_t *bmp_list;
extern int bmp_list_size;

void
bitmap_cleanup(bitmap_t *);

void
bitmap_draw_region(bitmap_t *, int, int, int, int, int, int, int);

void
bitmap_draw(bitmap_t *, int x, int y);

void
bitmap_draw_rotated(bitmap_t *, int, int, int, int, float, int);

void
bitmap_draw_textf(pixel_t, int, int, int, const char *, ...);

bitmap_t *
bitmap_get_target(bitmap_t *);

void
bitmap_set_target(bitmap_t *);

extern void *font;

#endif /* BITMAP_H_ */
