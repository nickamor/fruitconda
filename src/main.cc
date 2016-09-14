/*
 * main.c
 *
 *  Created on: May 30, 2011
 *      Author: nick
 */

#include <stdlib.h>
#include <stdio.h>

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "main.h"
#include "about.h"
#include "util.h"
#include "bitmap.h"
#include "state.h"
#include "gameplay.h"
#include "input.h"

#define SCALE_NOSTRETCH

// global allegro state variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *render_timer = NULL;

ALLEGRO_BITMAP *backbuffer_a;
ALLEGRO_BITMAP *backbuffer_b;
int backbuffer_flip = 0;

// global game state variables
int redraw = 1;
int game = 1;
int vdisplayx = 0, vdisplayy = 0;
float vdisplayscale = 1.0;

ALLEGRO_BITMAP *
backbuffer(void);

// TODO Genericise snake-related functions
// TODO Rename
// TODO Move video settings into bitmap.h
// TODO Refactor bitmap.h to be general video stuff
// TODO Stop using unportable key definitions
// TODO Other objects
// TODO Points & goal
// TODO AI/pathfinding
// TODO Fancier level select
// TODO Level editor
// TODO Network multiplay

double start_time = 0.0;
double delta_time = 0.0;

bitmap_t current_bgtile_bitmap;

struct video_settings_t video_settings =
        {0, 0, 0, 0, 0, 0};

int main(void);

int main() {
    if (!init()) {
        destroy();
        return 1;
    }

    gameplay_state.video_settings = &video_settings;
    change_state(mainmenu);

    while (game) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        switch (ev.type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                input_update(ev.keyboard.keycode);
                game_state.state_input();
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                change_state(nullstate);
                break;
            case ALLEGRO_EVENT_TIMER:
                game_state.state_update(delta_time);
                background_update();
                graphics_update();
                break;
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            render_buffer();
        }

        if (game_state.current_state == nullstate) {
            game = false;
        }
    }

    destroy();

    return 0;
}

/* int init()
 * sets up everything we need for the game
 */
int
init() {
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY);

    if (!al_init()) {
        error_message("failed to initialise allegro");
        return 0;
    }

    if (!al_init_image_addon()) {
        error_message("failed to initialise allegro_image");
        return 0;

    }

    al_init_font_addon();
    al_init_ttf_addon();

    video_settings.render_width = DEFAULT_DISPLAY_W;
    video_settings.render_height = DEFAULT_DISPLAY_H;
    video_settings.width = VIRTUAL_DISPLAY_W;
    video_settings.height = VIRTUAL_DISPLAY_H;
    video_settings.vsync = 0;
    video_settings.fullscreen = 1;

    if (video_settings.fullscreen == 1) {
        ALLEGRO_MONITOR_INFO info;
        al_get_monitor_info(0, &info);

        video_settings.render_width = info.x2 - info.x1;
        video_settings.render_height = info.y2 - info.y1;
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    }
    if (video_settings.vsync) {
        al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    }

    display = al_create_display(video_settings.render_width,
                                video_settings.render_height);
    if (!display) {
        error_message("failed to create display");
        return 0;
    }

    ALLEGRO_BITMAP *icon = al_load_bitmap("assets/graphics/icon.png");
    al_set_display_icon(display, icon);
    al_destroy_bitmap(icon);
    al_set_window_title(display, GAME_TITLE);

    al_install_keyboard();

    render_timer = al_create_timer(1.0 / MAX_FPS);
    if (!render_timer) {
        error_message("failed to create timer");
        return -1;
    }

    const char *font_filename = "assets/fonts/droidsans/mono.ttf";
    font = al_load_ttf_font(font_filename, -13, ALLEGRO_TTF_MONOCHROME);
    if (!font) {
        file_error_message("failed to load font", font_filename);
        return 0;
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        error_message("failed to create event queue");
        return 0;
    }
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue,
                             al_get_timer_event_source(render_timer));

    al_start_timer(render_timer);

// load bitmaps
    backbuffer_a = al_create_bitmap(VIRTUAL_DISPLAY_W, VIRTUAL_DISPLAY_H);
    backbuffer_b = al_create_bitmap(VIRTUAL_DISPLAY_W, VIRTUAL_DISPLAY_H);

    init_scale_settings();

    gameplay_state.tiles_info.tile_size = 16;

    memset(&current_bgtile_bitmap, 0, sizeof(bitmap_t));
    bitmap_from_file(&current_bgtile_bitmap, "assets/graphics/background1.png",
                     "background1");

    return 1;
}

/** void init_scale_settings()
 * calculates the scaling factor of the game screen
 */
void
init_scale_settings() {
    vdisplayscale = (float) al_get_display_width(display) / VIRTUAL_DISPLAY_W;

#ifdef SCALE_NOSTRETCH
    vdisplayscale = (int) vdisplayscale;
#endif

    vdisplayx = (al_get_display_width(display) / 2)
                - (VIRTUAL_DISPLAY_W / 2) * vdisplayscale;
    vdisplayy = (al_get_display_height(display) / 2)
                - (VIRTUAL_DISPLAY_H / 2) * vdisplayscale;

}

/** void draw()
 * main game rendering function
 */
void
render_buffer() {
    al_set_target_bitmap(backbuffer());

    al_clear_to_color(al_map_rgb(0, 0, 0));

    background_draw();

    game_state.state_draw(delta_time);

// final screen draw
    al_set_target_bitmap(al_get_backbuffer(display));
    al_draw_scaled_bitmap(backbuffer(), 0, 0, VIRTUAL_DISPLAY_W,
                          VIRTUAL_DISPLAY_H, vdisplayx, vdisplayy,
                          VIRTUAL_DISPLAY_W * vdisplayscale, VIRTUAL_DISPLAY_H * vdisplayscale, 0);
    al_flip_display();
    backbuffer_flip = !backbuffer_flip;
}

/** void draw_background(int dx, int dy)
 * used in draw() to render the scrolling tile background
 * $ dx, dy - an amount to offset the background by in x and y values
 */
float bg_scroll_x = 0.0, bg_scroll_y = 0.0;
float bg_scroll_x_v = 0.01, bg_scroll_y_v = 0.01;

void
background_update() {
    // scrolling values
    bg_scroll_x += bg_scroll_x_v;
    bg_scroll_y += bg_scroll_y_v;
    if (bg_scroll_x > 1.0) {
        bg_scroll_x = -1.0f;
    }
    if (bg_scroll_y > 1.0) {
        bg_scroll_y = -1.0f;
    }
    if (bg_scroll_x < -1.0) {
        bg_scroll_x = 1.0;
    }
    if (bg_scroll_y < -1.0) {
        bg_scroll_y = 1.0;
    }
}

void
background_draw() {
    ALLEGRO_BITMAP *bg_tile;
    bg_tile = (ALLEGRO_BITMAP *) current_bgtile_bitmap.data;

    int ts = 32;

    int max_x = (VIRTUAL_DISPLAY_W / ts) + 1;
    int max_y = (VIRTUAL_DISPLAY_H / ts) + 1;

    int y = -1;
    while (y <= max_y) {
        int x = -1;
        while (x <= max_x) {
            int dx = ts * bg_scroll_x;
            int dy = ts * bg_scroll_y;
            al_draw_bitmap(bg_tile, (x * ts) + dx, (y * ts) + dy, 0);

            ++x;
        }

        ++y;
    }
}

/** void update()
 * game tick logic
 */
void
graphics_update() {
    delta_time = al_get_time() - start_time;
    start_time = al_get_time();

    redraw = true;
}

/** void destroy()
 * cleans up some stuff (i.e. before we exit)
 */
void
destroy() {
    al_destroy_bitmap((ALLEGRO_BITMAP *) current_bgtile_bitmap.data);

// destroy buffers
    if (!backbuffer_a)
        al_destroy_bitmap(backbuffer_a);
    if (!backbuffer_b)
        al_destroy_bitmap(backbuffer_b);

    al_destroy_timer(render_timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

ALLEGRO_BITMAP *
backbuffer() {
    if (!backbuffer_flip) {
        return backbuffer_a;
    } else {
        return backbuffer_b;
    }
}
