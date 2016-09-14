/*
 * mainmenu.c
 *
 *  Created on: Sep 22, 2011
 *      Author: nick
 */

#include "about.h"
#include "input.h"
#include "menu.h"
#include "state.h"

char *mainmenu_menuitems[3] =
        {"Play", "About", "Quit"};

struct menu_s mainmenu_menu =
        {3, 0, GAME_TITLE, mainmenu_menuitems};

void
mainmenu_init(void) {
}

void
mainmenu_cleanup(void) {
}

void
mainmenu_pause(void) {
}

void
mainmenu_resume(void) {
}

void
mainmenu_input(void) {
    if (input_state.up) {
        menu_change_selection(&mainmenu_menu, -1);
    } else if (input_state.down) {
        menu_change_selection(&mainmenu_menu, 1);
    } else if (input_state.button_action) {
        switch (mainmenu_menu.selection) {
            case 0:
                change_state(levelselect);
                break;
            case 2:
                change_state(nullstate);
                break;
            default:
                break;
        }
    } else if (input_state.button_back) {
        change_state(nullstate);
    }
}

void
mainmenu_update(double) {
}

void
mainmenu_draw(double) {
    menu_draw(&mainmenu_menu, 100, 0);
}
