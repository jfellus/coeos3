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
	Rectangle bounds;
public:
	SVGComponent(const char* filename) { svg = new SVG; load(filename);}
	virtual ~SVGComponent() {delete svg;}


	void load(const char* filename) {svg->load(filename);}


	virtual Rectangle get_bounds() {
		if(!bounds) compute_bounds();
		return bounds + Vector2D(x,y);
	}

	virtual void render(Graphics& g) {
		g.drawSVG(*svg);
	}

	virtual void select(bool single) {
		Component::select(single);
	}

protected:
	void compute_bounds() {
		bounds = svg->get_bounds();
	}

	virtual void dump(std::ostream& os) { os << "SVGComponent(" << svg->filename << ")";}
};


#endif /* SVGCOMPONENT_H_ */
