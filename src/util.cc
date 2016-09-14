/*
 * util.c
 *
 *  Created on: 31/05/2011
 *      Author: nick
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "allegro5/allegro_native_dialog.h"
#include "util.h"

void *
safe_malloc(size_t size) {
    int *m;
    if ((m = (int *)calloc(1, size)) != NULL) {
        memset(m, 0, size);
        return m;
    } else {
        error_message("Could not allocate memory");
        return NULL;
    }
}

/** int file_exists(const char *filename)
 * tests to see if a file with the given filename exists
 * $ const char *filename - the filename to test
 * returns 1 if a file is found, otherwise 0
 */
int
file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

/** void error_message(const char *message)
 * displays a given error message in dialog and stderr form
 * $ message - the message to display
 */
void
error_message(const char *message) {
    fprintf(stderr, "Error: %s.\n", message);
    al_show_native_message_box(NULL, "Error", "A program error occurred.",
                               message, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

/** void file_error_message(const char *message, const char *filename)
 * displays a given error message and filename in dialog and stderr form
 * really just wraps error_message(*char) to take two strings
 * $ message - the message to display
 * $ filename - the filename to reference
 */
void
file_error_message(const char *message, const char *filename) {
    char text[128];
    sprintf(text, "%s: %s", message, filename);
    error_message(text);
}

const char *
shortname_from_filename(const char *filename) {
    char *tok, *prevtok = NULL;
    tok = strtok((char *) filename, "/.");
    while (tok != NULL) {
        prevtok = tok;
        tok = strtok(NULL, "/.");
    }
    return prevtok;
}
