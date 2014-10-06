/*
 * Color.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "../util/utils.h"

#define RGB_BLACK RGB(0,0,0)
#define RGB_WHITE RGB(1,1,1)
#define RGB_RED RGB(1,0,0)
#define RGB_GREEN RGB(0,1,0)
#define RGB_BLUE RGB(0,0,1)
#define RGB_INVALID RGB(-1,-1,-1)

class RGB {
public:
	unsigned char r,g,b;
public:
	RGB() {this->r = this->g = this->b = 0;}
	RGB(double r, double g, double b) { this->r = (unsigned char)r*255; this->g = (unsigned char)g*255; this->b = (unsigned char)b*255; }
	RGB(uint c) {
		this->r = (unsigned char)(c & 255);
		this->g = (unsigned char)((c >> 8) & 255);
		this->b = (unsigned char)((c >> 16) & 255);
	}

	uint as_packed_RGB() {
		return (((uint)r) << 16) | (((uint)g) << 8) | (((uint)b));
	}

	inline bool operator==(const RGB& c) const {return c.r==r && c.g==g && c.b==b;}
	inline bool operator!=(const RGB& c) const {return !(c == *this);}
};




#endif /* COLOR_H_ */
