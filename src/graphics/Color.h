/*
 * Color.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef COLOR_H_
#define COLOR_H_

#define RGB_BLACK RGB(0,0,0)
#define RGB_WHITE RGB(1,1,1)


class RGB {
public:
	double r,g,b;
public:
	RGB(double r, double g, double b) { this->r = r; this->g = g; this->b = b; }
};




#endif /* COLOR_H_ */
