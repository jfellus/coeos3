/*
 * WidgetGraphics.h
 *
 *  Created on: 1 oct. 2014
 *      Author: jfellus
 */

#ifndef WIDGETGRAPHICS_H_
#define WIDGETGRAPHICS_H_

#include "Graphics.h"


class WidgetGraphics : public Graphics {
public:
	WidgetGraphics(Cairo::Context* cr) { this->cr = cr; }
	virtual ~WidgetGraphics() { this->cr = 0; /* Avoid destruction of cr */}
};

#endif /* WIDGETGRAPHICS_H_ */
