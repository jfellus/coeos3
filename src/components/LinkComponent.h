/*
 * LinkComponent.h
 *
 *  Created on: 3 oct. 2014
 *      Author: jfellus
 */

#ifndef LINKCOMPONENT_H_
#define LINKCOMPONENT_H_

#include "Component.h"
#include "../util/geom.h"

class LinkComponent : public Component {
public:
	Component *src, *dst;
	Bezier* b;
public:
	LinkComponent(Component* src, Component* dst) {
		selectionLayer = -1;
		connect(src, dst);
		b = new Bezier(0,0,0,1,0,1,1,1);
	}
	virtual ~LinkComponent() {}

	void connect(Component* src, Component* dst) {
		this->src = src;
		this->dst = dst;
	}

	virtual void transform(Graphics& g) {} // No transform !

	Bezier bezier_absolute() {
		Vector2D cs = src->center(); Vector2D cd = dst->center();
		return Bezier(cs.x, cs.y, 	cs.x*(1-b->x2) + cd.x*b->x2, cs.y*(1-b->y2) + cd.y*b->y2,
						cs.x*(1-b->x3) + cd.x*b->x3, cs.y*(1-b->y3) + cd.y*b->y3,   cd.x, cd.y);
	}

	virtual Rectangle get_bounds() {
		return bezier_absolute().get_bounds();
	}

	virtual void render(Graphics& g);
	virtual bool hasPoint(double x, double y);

};

#endif /* LINKCOMPONENT_H_ */
