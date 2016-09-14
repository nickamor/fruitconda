/*
 * main.h
 *
 *  Created on: Aug 9, 2011
 *      Author: nick
 */

#ifndef MAIN_H_
#define MAIN_H_

// macros
#define MAX_FPS 60
#define DEFAULT_DISPLAY_W 800
#define DEFAULT_DISPLAY_H 600
#define VIRTUAL_DISPLAY_W 400
#define VIRTUAL_DISPLAY_H 300

// functions
int
init(void);

void
render_buffer(void);

void
graphics_update(void);

void
destroy(void);

// additional functions
void
init_scale_settings(void);

void
background_draw(void);

void
background_update(void);

#endif /* MAIN_H_ */
