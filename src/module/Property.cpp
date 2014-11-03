/*
 * Property.cpp
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */




#include "Property.h"



std::ostream& operator<<(std::ostream& os, Properties* a) {
	if(!a) {return os << "";}
	a->dump(os);
	return os;
}


std::ostream& operator<<(std::ostream& os, Property* a) {
	if(!a) {return os << "(null)";}
	a->dump(os);
	return os;
}
