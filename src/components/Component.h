/*
 * Component.h
 *
 *  Created on: 2 oct. 2014
 *      Author: jfellus
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "../widget/ZoomableDrawingArea.h"

class ZoomableDrawingArea;

class Component {
	double x,y;
	ZoomableDrawingArea* canvas;
public:
	Component() {x=y=0; canvas = 0;}
	virtual ~Component() {}

	void set_canvas(ZoomableDrawingArea* c) {canvas = c;}

	void set_pos(double x, double y) { this->x = x; this->y = y; repaint();}

	virtual void render(const Cairo::RefPtr<Cairo::Context>& cr) = 0;

	virtual void transform(const Cairo::RefPtr<Cairo::Context>& cr) {
		cr->translate(x,y);
	}

	void repaint();};

#endif /* COMPONENT_H_ */
