/*
 * utils.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#include "utils.h"
#include <glib-2.0/glib-object.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


static bool utils_init();
static bool inited = utils_init();
bool utils_init() {
	(void) inited;
	g_type_init();
	return true;
}

