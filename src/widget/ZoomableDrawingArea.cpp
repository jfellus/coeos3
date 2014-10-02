/*
 * ZoomableDrawingArea.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#include "ZoomableDrawingArea.h"
#include "../util/utils.h"
#include <glibmm.h>
#include <gdkmm.h>
#include <gdkmm/types.h>

ZoomableDrawingArea::ZoomableDrawingArea() {
	_zoom = 1;
	offsetx = offsety = 0;

	set_events(Gdk::ALL_EVENTS_MASK);
	add_events( Gdk::KEY_PRESS_MASK);
	signal_draw().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_draw), false);
	signal_scroll_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_scroll), false);
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_mouse_move), false);
	signal_key_press_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_key), false);
//	signal_realize().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_realize), false);
	grab_focus();

	svg = new SVG("test.svg");
}

ZoomableDrawingArea::~ZoomableDrawingArea() {

}


void ZoomableDrawingArea::on_realizes() {
}

bool ZoomableDrawingArea::on_key(GdkEventKey* event) {
	printf("ok\n");
	return true;
}

bool ZoomableDrawingArea::on_mouse_move(GdkEventMotion* event) {
	static double oldx=0,oldy=0;
	if(event->state & GDK_BUTTON1_MASK) {
		move(event->x-oldx, event->y-oldy);
	}
	oldx = event->x; oldy = event->y;
	return true;
}

bool ZoomableDrawingArea::on_scroll(GdkEventScroll* event) {
	double dx, dy;
	gdk_event_get_scroll_deltas((const GdkEvent*)event, &dx, &dy);
	zoom(-0.1 * dy, event->x, event->y);
	return true;
}


bool ZoomableDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	cr->set_source_rgb(1,1,1);
	cr->paint();

	cr->translate(offsetx, offsety);
	cr->scale(_zoom, _zoom);

	for(uint i=0; i<components.size(); i++) {
		cr->save();
		components[i]->transform(cr);
		components[i]->render(cr);
		cr->restore();
	}

	return FALSE;
}



void ZoomableDrawingArea::zoom(float fzoom, double cx, double cy) {
	cx =  (cx - offsetx)/ _zoom ;
	cy =  (cy - offsety)/ _zoom ;
	double oldzoom = _zoom;
	_zoom *= 1 + fzoom;
	offsetx -= cx*(_zoom - oldzoom);
	offsety -= cy*(_zoom - oldzoom);
	queue_draw();
}


void ZoomableDrawingArea::add(Component* c) {components.push_back(c); c->set_canvas(this); queue_draw();}
