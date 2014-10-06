/*
 * LinkComponent.cpp
 *
 *  Created on: 3 oct. 2014
 *      Author: jfellus
 */

#include "LinkComponent.h"
#include "../util/geom.h"
#include "../widget/ZoomableDrawingArea.h"

bool LinkComponent::hasPoint(double x, double y) {
	//double d = Line2D(src->center(),dst->center()).distance(Vector2D(x,y));
	//return d >= 0 && d*canvas->get_zoom() < 10;
	return bezier_absolute().has_point(x,y, 100/canvas->get_zoom());
}

void LinkComponent::render(Graphics& g) {
	Rectangle r = get_bounds(); r.augment(10);
	g.rectangle(r);
	g.rectangle(src->get_bounds().reverse());
	g.cr->clip();
	g.rectangle(r);
	g.rectangle(dst->get_bounds().reverse());
	g.cr->clip();

	g.set_color(selected ? RGB_RED : RGB_BLACK);
		g.bezier(bezier_absolute());
		g.stroke_line_width_independent(1);
		//g.drawLine(src->center(), dst->center());

}
