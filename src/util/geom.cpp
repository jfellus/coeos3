/*
 * geom.cpp
 *
 *  Created on: 2 oct. 2014
 *      Author: jfellus
 */


#include "geom.h"
#include "../graphics/Graphics.h"

int sgn(double s) {
	return s > 0 ? 1 : ((s<0) ? -1 : 0);
}

Vector2D Rectangle::center() {return Vector2D(x+w/2,y+h/2);}

Rectangle Bezier::get_precise_bounds() {
	Graphics g;
	Rectangle r;
	g.cr->set_line_width(3);
	g.bezier(*this);
	g.cr->get_stroke_extents(r.x, r.y, r.w, r.h);
	r.w -= r.x; r.h -= r.y;
	return r;
}

Rectangle Bezier::get_bounds() {
	Rectangle r(x1,y1,x4-x1,y4-y1);
	r.add(x2,y2);
	r.add(x3,y3);
	return r;
}


std::ostream& operator<<(std::ostream& os, const Rectangle& r)	{
	os << "(" << r.x << " " << r.y << " " << r.w << " " << r.h << ")";
	return os;
}


bool Bezier::has_point(double x, double y, double tolerance) {
	Graphics g;
	g.bezier(*this);
	return g.is_in_fill_or_stroke(x,y,false, true, tolerance);
}
