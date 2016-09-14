/*
 * input.c
 *
 *  Created on: Sep 22, 2011
 *      Author: nick
 */

#include "input.h"

struct input_state_t input_state =
        {0, 0, 0, 0, 0, 0, 0, 0, 0};

void
input_update(int keycode) {
    input_state.axis_x = 0;
    input_state.axis_y = 0;
    input_state.button_action = 0;
    input_state.button_back = 0;
    input_state.button_pause = 0;
    input_state.up = 0;
    input_state.down = 0;
    input_state.left = 0;
    input_state.right = 0;

    switch (keycode) {
        case GAMEKEY_UP:
            input_state.axis_y = 1;
            break;
        case GAMEKEY_DOWN:
            input_state.axis_y = -1;
            break;
        case GAMEKEY_LEFT:
            input_state.axis_x = -1;
            break;
        case GAMEKEY_RIGHT:
            input_state.axis_x = 1;
            break;
        case GAMEKEY_ACTION:
            input_state.button_action = 1;
            break;
        case GAMEKEY_BACK:
            input_state.button_back = 1;
            break;
        case GAMEKEY_PAUSE:
            input_state.button_pause = 1;
            break;
    }

    if (input_state.axis_x == -1) {
        input_state.left = 1;
    }
    if (input_state.axis_x == 1) {
        input_state.right = 1;
    }
    if (input_state.axis_y == 1) {
        input_state.up = 1;
    }
    if (input_state.axis_y == -1) {
        input_state.down = 1;
    }
}
