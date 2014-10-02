/*
 * string.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef STRING_H_
#define STRING_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool str_ends_with(const char* s, const char* suffix);
bool str_starts_with(const char* s, const char* prefix);
char* str_to_lower(const char* s);


void parse_keycode_str(const char* keycode_str, uint* key, uint* modifers);

#endif /* STRING_H_ */
