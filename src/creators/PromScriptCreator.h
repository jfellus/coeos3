/*
 * PromScriptCreator.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef PROMSCRIPTCREATOR_H_
#define PROMSCRIPTCREATOR_H_

#include <creator/Creator.h>
#include "../promethe/PromProject.h"
#include "../commands/CommandPromScriptCreate.h"


using namespace libboiboites;
namespace coeos {



class PromScriptCreator : public Creator {
public:
	PromProject* project;
	bool bKeep = false;
	ModulePromGroup* dummy_group = NULL;
	GroupPromScript* script = NULL;
public:
	PromScriptCreator(PromProject* project) : project(project) {}

	virtual void start(ZoomableDrawingArea* canvas);

	virtual void create(double x, double y);
	virtual void end() ;
	virtual void render(Graphics& g) {}

	virtual void on_mouse_move(GdkEventMotion* e) {
		Creator::on_mouse_move(e);
		dummy_group->component->center(Vector2D(x,y));
	}
};


}



#endif /* PROMSCRIPTCREATOR_H_ */
