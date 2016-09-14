/*
 * list.c
 *
 *  Created on: May 30, 2011
 *      Author: nick
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "util.h"

list_t *
list_init(list_t *list) {
    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;
    return list;
}

list_t *
list_create() {
    list_t *new_list;
    new_list = (list_t *) safe_malloc(sizeof(list_t));
    list_init(new_list);
    return new_list;
}

list_t *
list_append(list_t *list, void *data) {
    list_t *new_list = list_create();
    list_t *prev = list_foot(list);
    new_list->data = data;

    if (prev) {
        prev->next = new_list;
        new_list->prev = prev;
    } else {
        list = new_list;
    }

    return list;
}

/*
 * add to head
 */
list_t *
list_push(list_t *list, void *data) {
    list_t *
            new_list = list_create();
    new_list->data = data;

    if (list) {
        new_list->next = list;
        list->prev = new_list;
    }
    list = new_list;
    return list;
}

void *
list_find(list_t *list, void *data, int
(*fcompare)(const void *, const void *)) {
    if (list != NULL) {
        if (fcompare(data, list->data) == 0) {
            return list;
        } else {
            return list_find(list->next, data, fcompare);
        }
    } else {
        return NULL;
    }
}

list_t *
list_delete(list_t *list, void *data, int (*fcompare)(const void *, const void *)) {
    list_t *find;
    find = (list_t *) list_find(list, data, fcompare);

    if (find != NULL) {
        list_t *prev, *next;
        prev = find->prev;
        next = find->next;
        prev->next = next;
        next->prev = prev;
        free(find->data);
        free(find);
    }

    return list;
}

list_t *
list_foot(list_t *list) {
    if (list->next) {
        return list_foot(list->next);
    }

    return list;
}
