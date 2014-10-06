/*
 * Creator.cpp
 *
 *  Created on: 6 oct. 2014
 *      Author: jfellus
 */

#include "Creator.h"
#include "../widget/ZoomableDrawingArea.h"
#include "SVGComponent.h"

Creator::Creator() {
	comp = new SVGComponent("test.svg");
}

void Creator::start(ZoomableDrawingArea* canvas) {
	this->canvas = canvas;
}

void Creator::end() {
	canvas->end_creator();
}

void Creator::create(double x, double y) {
	canvas->add(comp);
	comp->set_pos(x,y);
	comp->set_selectable();

	end();
}

void Creator::render(Graphics& g) {
	g.set_color(RGB_RED);
	g.drawPoint(x,y);
}

void Creator::on_mouse_move(GdkEventMotion* e) {
	x = canvas->mouse_to_doc_x(e->x);
	y = canvas->mouse_to_doc_y(e->y);
	repaint();
}

void Creator::on_click(GdkEventButton* e) {

}

void Creator::on_unclick(GdkEventButton* e) {
	create(canvas->mouse_to_doc_x(e->x), canvas->mouse_to_doc_y(e->y));
}
