/*
 * utils.h
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#ifndef UTILS_H_
#define UTILS_H_



#define STRINGIFY(string) #string
#define STRINGIFY_CONTENT(variable) STRINGIFY(variable)
#define SCAN(size_max)  "%" STRINGIFY(size_max)
#define SCAN_STRING(size_max)  "%" STRINGIFY(size_max)"s"



#endif /* UTILS_H_ */
