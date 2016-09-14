/*
 * menu.h
 *
 *  Created on: Aug 31, 2011
 *      Author: nick
 */

#ifndef MENU_H_
#define MENU_H_

struct menu_s {
    int items;
    int selection;

    char *title;
    char **menuitems;
};

void
menu_change_selection(struct menu_s *, int);

void
menu_draw(struct menu_s *, int, int);

#endif /* MENU_H_ */
