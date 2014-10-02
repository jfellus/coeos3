/*
 * svg.cpp
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */


#include <librsvg/rsvg.h>

#include "svg.h"
#include "../util/utils.h"




struct _rsvgdata_t {
	RsvgHandle *handle;
};

static bool _init();
static bool inited = _init();
bool _init() {
	(void) inited;
	rsvg_set_default_dpi (72.0);
	return true;
}

void SVG::load(const char* filename) {
	GError *error = NULL;
	rsvg->handle = rsvg_handle_new_from_file (filename, &error);
	if (error != NULL) FAIL (error->message);
}

void SVG::init() {
	rsvg = new _rsvgdata_t;
}


void SVG::get_dim(double* w, double* h) {
	RsvgDimensionData dim;
	rsvg_handle_get_dimensions (rsvg->handle, &dim);
	*w = dim.width;
	*h = dim.height;
}

double SVG::get_width() {
	RsvgDimensionData dim;
	rsvg_handle_get_dimensions (rsvg->handle, &dim);
	return dim.width;
}

double SVG::get_height() {
	RsvgDimensionData dim;
	rsvg_handle_get_dimensions (rsvg->handle, &dim);
	return dim.height;
}

void SVG::render(cairo_t* cr) {
    rsvg_handle_render_cairo (rsvg->handle, cr);
}

void SVG::save(const char* filename) {
	if(file_has_ext(filename, ".png")) {
		GraphicsImage cr(get_width(),get_height());
	    render(((Cairo::Context&)cr).cobj());
	    cr.save(filename);
	}
}
