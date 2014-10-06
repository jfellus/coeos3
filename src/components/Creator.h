/*
 * Creator.h
 *
 *  Created on: 6 oct. 2014
 *      Author: jfellus
 */

#ifndef CREATOR_H_
#define CREATOR_H_

#include "Component.h"
#include <gtkmm.h>


class Creator : public Component {
protected:
	Component* comp;

public:
	Creator();
	virtual ~Creator() {}

public:

	void start(ZoomableDrawingArea* canvas);
	virtual void create(double x, double y);
	void end();

	virtual void render(Graphics& g);

	virtual void on_mouse_move(GdkEventMotion* e);
	virtual void on_click(GdkEventButton* e);
	virtual void on_unclick(GdkEventButton* e);

	virtual void transform(Graphics& g) {} // No transform !
};

#endif /* CREATOR_H_ */
