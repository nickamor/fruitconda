/*
 * mainmenu.h
 *
 *  Created on: Sep 22, 2011
 *      Author: nick
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

/* the seven state functions */
void
mainmenu_init(void);

void
mainmenu_cleanup(void);

void
mainmenu_pause(void);

void
mainmenu_resume(void);

void
mainmenu_input(void);

void
mainmenu_update(double);

void
mainmenu_draw(double);

#endif /* MAINMENU_H_ */
