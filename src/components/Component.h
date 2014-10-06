/*
 * Component.h
 *
 *  Created on: 2 oct. 2014
 *      Author: jfellus
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "../util/geom.h"
#include "../graphics/Graphics.h"

class ZoomableDrawingArea;
class BoundingBox;

class Component {
public:
	double x,y;
protected:
	ZoomableDrawingArea* canvas;
	bool selected = false;
	BoundingBox* selectionBox = 0;

public:
	int selectionLayer = 0;

public:
	Component() {x=y=0; canvas = 0;}
	virtual ~Component() {}

	virtual void set_canvas(ZoomableDrawingArea* c) {canvas = c;}

	void set_pos(double x, double y) { this->x = x; this->y = y; repaint();}
	void set_selectable(bool b = true);

	virtual void render(Graphics& g) = 0;

	virtual Vector2D center() { return get_bounds().center();}

	virtual void transform(Graphics& g) {
		g.cr->translate(x,y);
	}

	void translate(double dx, double dy) {
		this->x += dx; this->y += dy; repaint();
	}

	virtual void draw(Graphics& g);

	void select();
	void unselect();
	void toggle_select();
	bool is_selected() {return selected;}

	virtual Rectangle get_bounds() { return Rectangle();}
	virtual bool hasPoint(double x, double y) {return get_bounds().contains(x,y); }

	void repaint();

};


#endif /* COMPONENT_H_ */
