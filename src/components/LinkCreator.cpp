/*
 * LinkCreator.cpp
 *
 *  Created on: 6 oct. 2014
 *      Author: jfellus
 */

#include "LinkCreator.h"
#include "LinkComponent.h"
#include "../widget/ZoomableDrawingArea.h"


LinkCreator::LinkCreator() {
}

void LinkCreator::create(double x, double y) {
	if(!src) {
		Component* c = canvas->get_selectable_component_at(x,y);
		if(!c) return;
		src = c;
		repaint();
	} else {
		Component* c = canvas->get_selectable_component_at(x,y);
		if(!c) return;
		dst = c;

		linkcomp = new LinkComponent(src, dst);
		canvas->add(linkcomp);
		linkcomp->set_selectable();

		end();
	}
}

void LinkCreator::render(Graphics& g) {
	g.set_color(src ? RGB_BLUE : RGB_RED);
	g.drawPoint(x,y);

	if(src) {
		g.drawLine(src->center(), Vector2D(x,y));
		g.drawRect(src->get_bounds());
	}
}



