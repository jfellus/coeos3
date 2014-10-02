/*
 * svg.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef SVG_H_
#define SVG_H_

#include "GraphicsImage.h"
#include <cairomm/cairomm.h>

struct _rsvgdata_t;

class SVG {
public:
	struct _rsvgdata_t*  rsvg;

	SVG() {init();}
	SVG(const char* filename) {init(); load(filename);}


	void init();
	void load(const char* filename);

	void get_dim(double* w, double* h);
	double get_width();
	double get_height();

	void render(cairo_t* cr);

	void save(const char* filename);
};



#endif /* SVG_H_ */
