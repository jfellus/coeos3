/*
 * Graphics.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */



#include "Graphics.h"
#include <cairo/cairo.h>
#include "svg.h"
#include "../widget/ZoomableDrawingArea.h"

static Cairo::RefPtr<Cairo::Context> create_dummy_cairo_context() {
	Cairo::RefPtr<Cairo::ImageSurface> s = Cairo::ImageSurface::create(NULL, Cairo::FORMAT_RGB24, 0, 0, 0);
	return Cairo::Context::create(s);
}

Graphics::Graphics() {
	dummy_context = create_dummy_cairo_context();
	cr = dummy_context.operator ->();
	cr->save();
	canvas = 0;
}

Graphics::~Graphics() {
}


void Graphics::drawSVG(SVG &svg) {
	svg.render(*this);
}



void Graphics::drawPoint(double x, double y) {
	cr->save();
	cr->set_line_cap(Cairo::LINE_CAP_ROUND);
	cr->move_to (x, y);
	cr->line_to (x, y);
	stroke_line_width_independent(10);
	cr->restore();
}

void Graphics::drawLine(double x1, double y1, double x2, double y2) {
	cr->move_to (x1, y1);
	cr->line_to (x2, y2);
	cr->close_path ();
	stroke_line_width_independent(1);
}

void Graphics::fill(RGB rgb) {
	set_color(rgb);
	cr->paint();
}

void Graphics::set_color(const RGB& rgb) {
	cr->set_source_rgb(rgb.r/255.0, rgb.g/255.0, rgb.b/255.0);
}


void Graphics::stroke_line_width_independent(double lw) {
	cr->save();
	cr->set_line_width(lw);
	cr->scale(1.0/canvas->get_zoom(),1.0/canvas->get_zoom());
	cr->stroke();
	cr->restore();
}

void Graphics::drawRect(double x, double y, double w, double h) {
	cr->save();
	cr->set_source_rgb(0.5,0.5,1);
	cr->set_line_width(1);
	cr->rectangle(x,y,w,h);
	cr->scale(1.0/canvas->get_zoom(),1.0/canvas->get_zoom());
	cr->stroke();
	cr->restore();
}

void Graphics::drawHandle(double x, double y) {
	cr->save();
	cr->set_source_rgb(0.5,0.5,1);
	cr->translate(x,y);
	cr->scale(1.0/canvas->get_zoom(),1.0/canvas->get_zoom());
	cr->rectangle(-2.5,-2.5,5,5);
	cr->fill();
	cr->restore();
}

void Graphics::map_color(RGB c_from, RGB c_to) {
	RGB* c2 = new RGB(c_to);
	colormap[c_from.as_packed_RGB()] = c2;
}

void Graphics::unmap_color(RGB c_from) {
	colormap.erase(c_from.as_packed_RGB());
}

RGB* Graphics::get_mapped_color(RGB* c) {
	if(!c) return NULL;
	if(colormap.count(c->as_packed_RGB())) return colormap[c->as_packed_RGB()];
	return c;
}

void Graphics::fill_and_stroke(RGB* fill, RGB* stroke) {
	if(colormap.size()) { fill = get_mapped_color(fill); stroke = get_mapped_color(stroke); }
	if(fill && stroke)  {
		set_color(*fill);
		cr->fill_preserve();
		set_color(*stroke);
		cr->stroke();
	}
	else if(!fill && stroke)  { set_color(*stroke); cr->stroke();}
	else if(fill && !stroke)  { set_color(*fill); cr->fill();}
}

Rectangle Graphics::fill_and_stroke_extents(bool fill, bool stroke) {
	Rectangle r;
	if(stroke)  cr->get_stroke_extents(r.x, r.y, r.w, r.h);
	else if(fill) cr->get_fill_extents(r.x, r.y, r.w, r.h);
	else return r;
	r.w -= r.x; r.h -= r.y;
	r.empty = false;
	return r;
}

bool Graphics::is_in_fill_or_stroke(double x, double y, bool fill, bool stroke, double tolerance) {
	cr->set_line_width(tolerance == 0 ? 1 : tolerance);
	bool b = false;
	if(stroke && fill) b = cr->in_fill(x,y) || cr->in_stroke(x,y);
	if(stroke)  b = cr->in_stroke(x,y);
	else if(fill) b = cr->in_fill(x,y);
	return b;
}


void Graphics::bezier(const Bezier& b) {
	cr->move_to(b.x1, b.y1);
	cr->curve_to(b.x2, b.y2, b.x3, b.y3, b.x4, b.y4);
}

void Graphics::line(const Line2D& l) {
	cr->move_to(l.a.x, l.a.y);
	cr->line_to(l.b.x, l.b.y);
}

void Graphics::rectangle(const Rectangle& r) {
	cr->rectangle(r.x, r.y, r.w, r.h);
}
