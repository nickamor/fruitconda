/*
 * menu.c
 *
 *  Created on: Aug 31, 2011
 *      Author: nick
 */

#include "menu.h"
#include "bitmap.h"

void
menu_change_selection(struct menu_s *menu, int change) {
    menu->selection += change;

    if (menu->selection >= menu->items) {
        menu->selection = 0;
    } else if (menu->selection < 0) {
        menu->selection = menu->items - 1;
    }
}

void
menu_draw(struct menu_s *menu, int x, int y) {
    pixel_t color =
            {255, 255, 255, 255};
    bitmap_draw_textf(color, x, y, 0, menu->title);

    int i = 0;
    while (i < menu->items) {
        if (i == menu->selection) {
            bitmap_draw_textf(color, x, y + 24 + i * 16, 0, "> %s",
                              menu->menuitems[i]);
        } else {
            bitmap_draw_textf(color, x, y + 24 + i * 16, 0, "%s",
                              menu->menuitems[i]);
        }
        ++i;
    }
}
