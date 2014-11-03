/*
 * tools.h
 *
 *  Created on: 19 oct. 2014
 *      Author: jfellus
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include "../util/utils.h"


extern const char* PROM_GROUP_TYPES[];


#define STRINGIFY(string) #string
#define STRINGIFY_CONTENT(variable) STRINGIFY(variable)
#define SCAN(size_max)  "%" STRINGIFY(size_max)
#define SCAN_STRING(size_max)  "%" STRINGIFY(size_max)"s"


bool LINK_TYPE_HAS_NAME(int link_type);




#endif /* TOOLS_H_ */
