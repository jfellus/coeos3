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
#include "../components/Creator.h"
#include <vector>
#include <algorithm>

class Component;
class BoundingBox;

enum SelectionRenderingMode {
	SELECTION_ShowComponentsBoundingBoxes,
	SELECTION_ShowSingleBoundingBox,
	SELECTION_Colored,
};

class ZoomableDrawingArea : public Gtk::DrawingArea {

	std::vector<Component*> components;
	std::vector<Component*> components_selectables;
	std::vector<Component*> selection;

	Creator* creator = 0;

	BoundingBox* selectionBox = 0;
	BoundingBox* multi_selectionBox = 0;
	Component* draggedComponent = 0;
	double drag_start_x = 0, drag_start_y = 0;
	bool bFirstDraw = true;
	Vector2D mousePosDoc;

	double _zoom;
	double offsetx, offsety;

	bool isDragging;

	SVG* svg;

public:
	enum SelectionRenderingMode selectionRenderingMode = SELECTION_Colored;

public:
	ZoomableDrawingArea(Gtk::Window& win);
	virtual ~ZoomableDrawingArea();


	int get_width() {  Gtk::Allocation allocation = get_allocation(); return allocation.get_width(); }
	int get_height() {  Gtk::Allocation allocation = get_allocation(); return allocation.get_height(); }

	double get_zoom() { return _zoom; }

	void add(Component* c);
	void remove(Component* c);

	void add_selectable(Component* c);
	void remove_selectable(Component* c);

	double mouse_to_doc_x(double x) {return (x-offsetx)/_zoom;}
	double mouse_to_doc_y(double y) {return (y-offsety)/_zoom;}
	double doc_to_mouse_x(double x) {return x*_zoom + offsetx; }
	double doc_to_mouse_y(double y) {return y*_zoom + offsety; }

	Vector2D get_mouse_pos_doc() {return mousePosDoc;}

	void zoom(float fzoom) {_zoom *= (1 + fzoom); queue_draw();}
	void move(double dx, double dy) {offsetx += dx; offsety += dy; queue_draw();}
	void zoom(float fzoom, double cx, double cy);

	void zoom_all();
	void zoom(Component* c);
	void zoom(const Rectangle& r);

	void translate_selection(double dx, double dy);


	void select(Component* c);
	void select(double x1, double y1, double x2, double y2);
	void unselect(Component* c);
	void unselect_all();
	void toggle_select(Component* c);
	void select_all();
	bool has_selection() {return !selection.empty();}

	Component* get_selectable_component_at(double x, double y);


	void start_creator(Creator* c);
	void end_creator();


	void repaint() {queue_draw();}


	void add_selection(Component* c);
	void remove_selection(Component* c);
protected:
	virtual bool on_click(GdkEventButton* event);
	virtual bool on_unclick(GdkEventButton* event);
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	virtual bool on_key(GdkEventKey* event);
	virtual bool on_mouse_move(GdkEventMotion* event);
	virtual bool on_scroll(GdkEventScroll* event);
	virtual void on_first_draw();
public:
	virtual void on_realizes();
};



#endif /* ZOOMABLEDRAWINGAREA_H_ */
