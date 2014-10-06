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
#include "../components/BoundingBox.h"
#include <gtkmm.h>
#include "../components/LinkCreator.h"

ZoomableDrawingArea::ZoomableDrawingArea(Gtk::Window& w) {
	isDragging = false;
	_zoom = 1;
	offsetx = offsety = 0;
	draggedComponent = 0;
	set_events(Gdk::ALL_EVENTS_MASK);
	w.add_events( Gdk::KEY_PRESS_MASK);
	signal_draw().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_draw), false);
	signal_scroll_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_scroll), false);
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_mouse_move), false);
	signal_button_press_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_click), false);
	signal_button_release_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_unclick), false);
	w.signal_key_press_event().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_key), false);
	signal_realize().connect(sigc::mem_fun(*this, &ZoomableDrawingArea::on_realize), false);
	grab_focus();

	svg = new SVG("test.svg");

	selectionBox = new BoundingBox(&selection);
	multi_selectionBox = new BoundingBox(Rectangle());
}

ZoomableDrawingArea::~ZoomableDrawingArea() {

}


void ZoomableDrawingArea::on_realizes() {
	grab_focus();
	zoom_all();
}

bool ZoomableDrawingArea::on_key(GdkEventKey* event) {
	if(event->state & GDK_CONTROL_MASK) {
		if(event->keyval==GDK_KEY_a) select_all();

		double a = 10;
		if(event->state & GDK_SHIFT_MASK) a *= 5;
		if(event->keyval==GDK_KEY_Left) move(a,0);
		if(event->keyval==GDK_KEY_Right) move(-a,0);
		if(event->keyval==GDK_KEY_Up) move(0,a);
		if(event->keyval==GDK_KEY_Down) move(0, -a);

	} else {
		if(event->keyval==GDK_KEY_z) zoom_all();


		double a = 3;
		if(event->state & GDK_SHIFT_MASK) a *= 5;
		if(event->keyval==GDK_KEY_Left) translate_selection(-a/_zoom, 0);
		if(event->keyval==GDK_KEY_Right) translate_selection(a/_zoom, 0);
		if(event->keyval==GDK_KEY_Up) translate_selection(0,-a/_zoom);
		if(event->keyval==GDK_KEY_Down) translate_selection(0,a/_zoom);

		if(event->keyval==GDK_KEY_g) start_creator(new Creator());
		if(event->keyval==GDK_KEY_l) start_creator(new LinkCreator());
	}
	return false;
}


bool ZoomableDrawingArea::on_click(GdkEventButton* event) {
	if(creator) {
		creator->on_click(event);
		return true;
	}

	if(event->button==1) {
		drag_start_x = mouse_to_doc_x(event->x);
		drag_start_y = mouse_to_doc_y(event->y);

		if(event->state & GDK_SHIFT_MASK) draggedComponent = 0;
		else {
			draggedComponent = get_selectable_component_at(drag_start_x, drag_start_y);
			if(!draggedComponent || !draggedComponent->is_selected()) {
				unselect_all();
				if(draggedComponent) select(draggedComponent);
			}
		}
	}
	return true;
}

bool ZoomableDrawingArea::on_unclick(GdkEventButton* event) {
	mousePosDoc.x = mouse_to_doc_x(event->x);
	mousePosDoc.y = mouse_to_doc_y(event->y);

	if(creator) {
		creator->on_unclick(event);
		return true;
	}

	if(event->button==1) {
		multi_selectionBox->set_bounds(Rectangle());

		if(!(isDragging && draggedComponent) && !(event->state & GDK_SHIFT_MASK)) unselect_all();
		if(isDragging && !draggedComponent) {
			select(drag_start_x, drag_start_y, mousePosDoc.x, mousePosDoc.y);
		}

		if(!isDragging && (event->state & GDK_SHIFT_MASK)) draggedComponent = get_selectable_component_at(mousePosDoc.x, mousePosDoc.y);
		if(!isDragging && draggedComponent) toggle_select(draggedComponent);

		draggedComponent = 0;
		isDragging = false;
		repaint();
	}
	return true;
}



bool ZoomableDrawingArea::on_mouse_move(GdkEventMotion* event) {
	static double oldx=0,oldy=0;
	mousePosDoc.x = mouse_to_doc_x(event->x);
	mousePosDoc.y = mouse_to_doc_y(event->y);

	if(creator) {
		creator->on_mouse_move(event);
		return true;
	}

	if(event->state & GDK_BUTTON1_MASK) {
		isDragging = true;
		if(has_selection() && !(event->state & GDK_SHIFT_MASK)) {
			translate_selection(mousePosDoc.x-oldx, mousePosDoc.y-oldy);
		} else {
			multi_selectionBox->set_bounds(Rectangle(drag_start_x, drag_start_y,mousePosDoc.x-drag_start_x, mousePosDoc.y-drag_start_y));
		}
		repaint();
	}
	oldx = mousePosDoc.x; oldy = mousePosDoc.y;
	return true;
}

bool ZoomableDrawingArea::on_scroll(GdkEventScroll* event) {
	double dx, dy;
	gdk_event_get_scroll_deltas((const GdkEvent*)event, &dx, &dy);
	if(event->state & GDK_CONTROL_MASK) zoom(-0.1 * dy, event->x, event->y);
	else move(20*dx, 20*dy);
	return true;
}

void ZoomableDrawingArea::on_first_draw() {
	zoom_all();
	bFirstDraw = false;
}

bool ZoomableDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
	if(bFirstDraw) on_first_draw();
	cr->set_source_rgb(1,1,1);
	cr->paint();

	cr->translate(offsetx, offsety);
	cr->scale(_zoom, _zoom);

	Graphics g(cr.operator ->(), this);
	g.set_color(RGB_BLACK);
	for(uint i=0; i<components.size(); i++) components[i]->draw(g);

	if(selectionRenderingMode==SELECTION_ShowSingleBoundingBox && selectionBox) selectionBox->draw(g);
	if(multi_selectionBox) multi_selectionBox->draw(g);
	if(creator) creator->draw(g);

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


static bool _selectionLayerComp(Component* c1, Component* c2) {
	return c1->selectionLayer < c2->selectionLayer;
}

void ZoomableDrawingArea::add(Component* c) {components.push_back(c); c->set_canvas(this); queue_draw();}
void ZoomableDrawingArea::remove(Component* c) {vector_remove(components,c); c->set_canvas(0); queue_draw();}
void ZoomableDrawingArea::add_selectable(Component* c) {
	components_selectables.push_back(c);
	std::sort (components_selectables.begin(), components_selectables.end(), _selectionLayerComp);
}
void ZoomableDrawingArea::remove_selectable(Component* c) {vector_remove(components_selectables,c);}


Component* ZoomableDrawingArea::get_selectable_component_at(double x, double y) {
	for(int i=components_selectables.size()-1; i>=0; i--) {
		Component* c = components_selectables[i];
		if(c->hasPoint(x,y)) return c;
	}
	return 0;
}

void ZoomableDrawingArea::select_all() {
	for(uint i=0; i<components_selectables.size(); i++) select(components_selectables[i]);
}

void ZoomableDrawingArea::select(Component* c) {
	DBG("Select " << c);
	c->select();
}

void ZoomableDrawingArea::toggle_select(Component* c) {
	c->toggle_select();
}


void ZoomableDrawingArea::select(double x1, double y1, double x2, double y2) {
	Rectangle r(x1,y1,x2-x1,y2-y1);
	for(uint i=0; i<components_selectables.size(); i++) {
		Component* c = components_selectables[i];
		if(c->get_bounds().is_in(r)) select(c);
	}
}

void ZoomableDrawingArea::unselect(Component* c) {
	c->unselect();
}

void ZoomableDrawingArea::unselect_all() {
	while(!selection.empty()) {
		DBG("Unselect" << selection[0]);
		selection[0]->unselect();
	}
	selectionBox->set_bounds(Rectangle());
	repaint();
}

void ZoomableDrawingArea::add_selection(Component* c) {
	selection.push_back(c);
	selectionBox->set_bounds(&selection);
	repaint();
}

void ZoomableDrawingArea::remove_selection(Component* c) {
	if(!vector_remove(selection, c)) DBG("Can't remove " << c);
	selectionBox->set_bounds(&selection);
	if(selection.empty()) selectionBox->set_bounds(Rectangle());
	repaint();
}


void ZoomableDrawingArea::translate_selection(double dx, double dy) {
	for(uint i=0; i<selection.size(); i++) {
		selection[i]->translate(dx,dy);
	}
}


void ZoomableDrawingArea::zoom_all() {
	if(components.empty()) return;
	Rectangle r = components[0]->get_bounds();
	for(uint i=1; i<components.size(); i++) r.add(components[i]->get_bounds());
	zoom(r);
}

void ZoomableDrawingArea::zoom(Component* c) {
	zoom(c->get_bounds());
}

void ZoomableDrawingArea::zoom(const Rectangle& r) {
	double w = get_allocated_width()-20;
	double h = get_allocated_height()-20;
	if(!w || !h) return;
	_zoom = MIN(h/r.h, w/r.w);

	offsetx= -r.x*_zoom + 10 + MAX(0,(w - r.w*_zoom)/2);
	offsety= -r.y*_zoom + 10 + MAX(0,(h - r.h*_zoom)/2);
	repaint();
}

void ZoomableDrawingArea::start_creator(Creator* c) {creator = c; c->start(this); repaint();}
void ZoomableDrawingArea::end_creator() {delete creator; creator = 0; repaint();}


