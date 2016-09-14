/*
 * state.h
 *
 *  Created on: Aug 30, 2011
 *      Author: nick
 */

#ifndef STATE_H_
#define STATE_H_

typedef enum GAME_STATE {
    mainmenu, levelselect, gameplay, gameover, about, nullstate = -1
} gamestate;

struct state_manager_t {
    gamestate current_state;
    // TODO: implement next_state (as opposed to change_state())
    // change state after the current states update?
    gamestate next_state;

    void
    (*state_init)();

    void
    (*state_cleanup)();

    void
    (*state_pause)();

    void
    (*state_resume)();

    void
    (*state_input)();

    void
    (*state_update)(double);

    void
    (*state_draw)(double);
};

typedef struct state_manager_t state_manager;

extern state_manager game_state;

void
change_state(gamestate);

#endif /* STATE_H_ */
