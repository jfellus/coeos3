/*
 * PromScript.cpp
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#include "PromScript.h"


std::ostream& operator<<(std::ostream& os, PromScript* a) {
	a->dump(os);
	return os;
}

