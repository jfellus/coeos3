/*
 * utils.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef UTILS_H_
#define UTILS_H_


#include "string.h"
#include "file.h"
#include "func_ptr.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#define FAIL(msg) do { fprintf (stderr, "FAIL: %s\n", msg); exit (-1); } while (0)

#define DBG(x) std::cerr << x << "\n";

#include <algorithm>

template <class T> bool vector_remove(std::vector<T>& v, T val) {
	typename std::vector<T>::iterator position = std::find(v.begin(), v.end(), val);
	if (position != v.end()) {v.erase(position); return true;}
	return false;
}

#endif /* UTILS_H_ */
