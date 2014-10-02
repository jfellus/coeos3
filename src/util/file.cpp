/*
 * file.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#include "file.h"
#include "string.h"


bool file_has_ext(const char* filename, const char* ext) {
	return str_ends_with(str_to_lower(filename),ext);
}
