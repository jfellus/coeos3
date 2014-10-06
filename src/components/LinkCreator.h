/*
 * LinkCreator.h
 *
 *  Created on: 6 oct. 2014
 *      Author: jfellus
 */

#ifndef LINKCREATOR_H_
#define LINKCREATOR_H_


#include "Creator.h"

class LinkComponent;

class LinkCreator : public Creator {
protected:
	Component* src = 0, *dst = 0;
	LinkComponent* linkcomp = 0;
public:
	LinkCreator();
	virtual ~LinkCreator() {}

	virtual void create(double x, double y);

	virtual void render(Graphics& g);
};

#endif /* LINKCREATOR_H_ */
