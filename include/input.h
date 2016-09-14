/*
 * input.h
 *
 *  Created on: Sep 22, 2011
 *      Author: nick
 */

#ifndef INPUT_H_
#define INPUT_H_

#define GAMEKEY_UP      84
#define GAMEKEY_DOWN    85
#define GAMEKEY_LEFT    82
#define GAMEKEY_RIGHT   83

#define GAMEKEY_ACTION  67
#define GAMEKEY_BACK    59
#define GAMEKEY_PAUSE   16

struct input_state_t {
    int axis_x;
    int axis_y;
    int button_action;
    int button_back;
    int button_pause;

    int up;
    int down;
    int left;
    int right;
};

extern struct input_state_t input_state;

void input_update(int);

#endif /* INPUT_H_ */
