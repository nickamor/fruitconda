/*
 * list.h
 *
 *  Created on: May 30, 2011
 *      Author: nick
 */

#ifndef LIST_H_
#define LIST_H_

typedef struct list_s list_t;

struct list_s {
    void *data;
    list_t *next;
    list_t *prev;
};

list_t *
list_init(list_t *list);

list_t *
list_create(void); //TODO rename

/*
 * add to tail
 */
list_t *
list_append(list_t *list, void *data);

list_t *
list_push(list_t *list, void *data);

void *
list_find(list_t *list, void *data, int
(*fcompare)(const void *, const void *));

list_t *
list_delete(list_t *list, void *data, int
(*fcompare)(const void *, const void *));

void
list_free(list_t *list);

list_t *
list_foot(list_t *list);

#endif /* LIST_H_ */
