/*
 * Graphics.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */



#include "Graphics.h"
#include <cairo/cairo.h>
#include "svg.h"

Graphics::~Graphics() {
}


void Graphics::drawSVG(SVG &svg) {
	svg.render(cr->cobj());
}


void Graphics::drawPoint(double x, double y) {
	cr->save();
	set_color(RGB_BLACK);
	cr->set_line_width(10);
	cr->set_line_cap(Cairo::LINE_CAP_ROUND);
	cr->move_to (x, y);
	cr->line_to (x, y);
	cr->stroke ();
	cr->restore();
}

void Graphics::drawLine(double x1, double y1, double x2, double y2) {
	set_color(RGB_BLACK);
	cr->move_to (x1, y1);
	cr->line_to (x2, y2);
	cr->close_path ();
	cr->stroke ();
}

void Graphics::fill(RGB rgb) {
	set_color(rgb);
	cr->paint();
}

void Graphics::set_color(RGB rgb) {
	cr->set_source_rgb(rgb.r, rgb.g, rgb.b);
}
