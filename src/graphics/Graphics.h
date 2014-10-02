/*
 * Graphics.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Color.h"
#include <cairomm/cairomm.h>

class SVG;

/** NOT INTENDED TO INSTANTIATION : USE SUBCLASSES */
class Graphics {
protected:
	Cairo::Context* cr;

public:
	Graphics() {cr = 0;}
	virtual ~Graphics();

	operator Cairo::Context&() const { return *cr;}


	// Drawing

	void drawGradient1();
	void drawGradient2();
	void drawGradient3();

	void drawSVG(SVG& svg);

	void drawPoint(double x, double y);
	void drawLine(double x1, double y1, double x2, double y2);


	void fill(RGB rgb);

	void set_color(RGB rgb);
};

#endif /* GRAPHICS_H_ */
