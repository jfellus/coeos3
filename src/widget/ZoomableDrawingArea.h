/*
 * ZoomableDrawingArea.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef ZOOMABLEDRAWINGAREA_H_
#define ZOOMABLEDRAWINGAREA_H_

#include <gtkmm/drawingarea.h>
#include "../graphics/svg.h"
#include "../components/Component.h"
#include <vector>

class Component;

class ZoomableDrawingArea : public Gtk::DrawingArea {

	std::vector<Component*> components;

	double _zoom;
	double offsetx, offsety;

	SVG* svg;

public:
	ZoomableDrawingArea();
	virtual ~ZoomableDrawingArea();


	int get_width() {  Gtk::Allocation allocation = get_allocation(); return allocation.get_width(); }
	int get_height() {  Gtk::Allocation allocation = get_allocation(); return allocation.get_height(); }

	void add(Component* c);


	void zoom(float fzoom) {_zoom *= (1 + fzoom); queue_draw();}
	void move(double dx, double dy) {offsetx += dx; offsety += dy; queue_draw();}
	void zoom(float fzoom, double cx, double cy);

	void repaint() {queue_draw();}
protected:
	virtual void on_realizes();
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	virtual bool on_key(GdkEventKey* event);
	virtual bool on_mouse_move(GdkEventMotion* event);
	virtual bool on_scroll(GdkEventScroll* event);


};

#endif /* ZOOMABLEDRAWINGAREA_H_ */
