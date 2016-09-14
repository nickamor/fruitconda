/*
 * list.h
 *
 *  Created on: May 31, 2011
 *      Author: nick
 */

#ifndef UTIL_H_
#define UTIL_H_

void *
safe_malloc(size_t size);

int
file_exists(const char *filename);

void
error_message(const char *message);

void
file_error_message(const char *message, const char *filename);

const char *
shortname_from_filename(const char *filename);

#endif /* LIST_H_ */
