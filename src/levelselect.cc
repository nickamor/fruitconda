/*
 * levelselect.c
 *
 *  Created on: Sep 22, 2011
 *      Author: nick
 */

#include "input.h"
#include "state.h"
#include "level.h"
#include "gameplay.h"
#include "menu.h"

level_t current_level;

char *level_filenames[2] =
        {"assets/levels/level1.bmp", "assets/levels/level2.bmp"};

char *levelselect_menuitems[2] =
        {"Level 1", "Level 2"};

struct menu_s levelselect_m =
        {2, 0, "Select Level", levelselect_menuitems};

void
levelselect_init(void) {
}

void
levelselect_cleanup(void) {
}

void
levelselect_pause(void) {
}

void
levelselect_resume(void) {
}

void
levelselect_input(void) {
    if (input_state.up) {
        menu_change_selection(&levelselect_m, -1);
    } else if (input_state.down) {
        menu_change_selection(&levelselect_m, 1);
    } else if (input_state.button_action) {
        level_load_fromfile(&current_level,
                            level_filenames[levelselect_m.selection]);

        gameplay_state.level = &current_level;
        change_state(gameplay);
    } else if (input_state.button_back) {
        change_state(mainmenu);
    }
}

void
levelselect_update(double) {
}

void
levelselect_draw(double) {
    menu_draw(&levelselect_m, 100, 0);
}
