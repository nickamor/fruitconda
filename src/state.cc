/*
 * state.c
 *
 *  Created on: Aug 30, 2011
 *      Author: nick
 */

#include <cstdio>
#include "state.h"
#include "gameplay.h"
#include "mainmenu.h"
#include "levelselect.h"

state_manager game_state =
        {mainmenu, mainmenu, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void
change_state(gamestate state) {
    if (game_state.state_cleanup) {
        game_state.state_cleanup();
    }

    game_state.current_state = state;

    switch (game_state.current_state) {
        case mainmenu:
            game_state.state_init = &mainmenu_init;
            game_state.state_cleanup = &mainmenu_cleanup;
            game_state.state_pause = &mainmenu_pause;
            game_state.state_resume = &mainmenu_resume;
            game_state.state_input = &mainmenu_input;
            game_state.state_update = &mainmenu_update;
            game_state.state_draw = &mainmenu_draw;
            break;
        case levelselect:
            game_state.state_init = &levelselect_init;
            game_state.state_cleanup = &levelselect_cleanup;
            game_state.state_pause = &levelselect_pause;
            game_state.state_resume = &levelselect_resume;
            game_state.state_input = &levelselect_input;
            game_state.state_update = &levelselect_update;
            game_state.state_draw = &levelselect_draw;
            break;
        case gameplay:
            game_state.state_init = &gameplay_init;
            game_state.state_cleanup = &gameplay_cleanup;
            game_state.state_pause = &gameplay_pause;
            game_state.state_resume = &gameplay_resume;
            game_state.state_input = &gameplay_input;
            game_state.state_update = &gameplay_update;
            game_state.state_draw = &gameplay_draw;
            break;
        case nullstate:
            return;
        default:
            break;
    }

    game_state.state_init();
}


