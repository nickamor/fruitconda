/*
 * levelselect.h
 *
 *  Created on: Sep 22, 2011
 *      Author: nick
 */

#ifndef LEVELSELECT_H_
#define LEVELSELECT_H_

/* the seven state functions */
void
levelselect_init(void);

void
levelselect_cleanup(void);

void
levelselect_pause(void);

void
levelselect_resume(void);

void
levelselect_input(void);

void
levelselect_update(double);

void
levelselect_draw(double);

#endif /* LEVELSELECT_H_ */
