/*
 * Graphics.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Color.h"
#include <cairomm/cairomm.h>
#include "../util/geom.h"
#include <map>

class SVG;
class ZoomableDrawingArea;
class ColorMap;

/** NOT INTENDED TO INSTANTIATION : USE SUBCLASSES */
class Graphics {
public:
	std::map<uint,RGB*> colormap;
	Cairo::Context* cr;
	ZoomableDrawingArea* canvas;
private:
	Cairo::RefPtr<Cairo::Context> dummy_context;
public:
	Graphics(); // Create dummy Graphics for extents measurements
	Graphics(Cairo::Context* cr, ZoomableDrawingArea* a) {this->cr = cr; canvas = a;}
	virtual ~Graphics();

	operator Cairo::Context&() const { return *cr;}


	// Drawing

	void drawGradient1();
	void drawGradient2();
	void drawGradient3();

	void drawSVG(SVG& svg);

	void drawPoint(const Vector2D& p) {drawPoint(p.x,p.y);}
	void drawPoint(double x, double y);
	void drawLine(const Vector2D& a, const Vector2D& b) {drawLine(a.x, a.y, b.x, b.y);}
	void drawLine(double x1, double y1, double x2, double y2);
	void drawRect(double x, double y, double w, double h);
	void drawRect(const Rectangle& r) {drawRect(r.x, r.y, r.w, r.h);}
	void drawHandle(double x, double y);
	void drawCubicBez(float* p);

	void stroke_line_width_independent(double lw);

	void map_color(RGB c_from, RGB  c_to);
	void unmap_color(RGB c_from);
	RGB* get_mapped_color(RGB* c);

	void bezier(const Bezier& b);
	void line(const Line2D& l);
	void rectangle(const Rectangle& r);

	void fill(RGB rgb);

	void set_color(const RGB& rgb);

	void fill_and_stroke(RGB* fill, RGB* stroke = 0);
	Rectangle fill_and_stroke_extents(bool fill, bool stroke);
	bool is_in_fill_or_stroke(double x, double y, bool fill, bool stroke, double tolerance);
};

#endif /* GRAPHICS_H_ */
