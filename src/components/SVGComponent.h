/*
 * SVGComponent.h
 *
 *  Created on: 2 oct. 2014
 *      Author: jfellus
 */

#ifndef SVGCOMPONENT_H_
#define SVGCOMPONENT_H_

#include "Component.h"
#include "../graphics/svg.h"


class SVGComponent : public Component {
	SVG* svg;
public:
	SVGComponent(const char* filename) { svg = new SVG; load(filename);}
	virtual ~SVGComponent() {delete svg;}


	void load(const char* filename) {svg->load(filename);}

	virtual void render(const Cairo::RefPtr<Cairo::Context>& cr) {
		svg->render(cr->cobj());
	}
};

#endif /* SVGCOMPONENT_H_ */
