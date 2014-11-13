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

class PromScriptCreator : public Creator {
public:
	PromProject* project;
	bool bKeep = false;
	ModulePromGroup* dummy_group = NULL;
	GroupPromScript* script = NULL;
public:
	PromScriptCreator(PromProject* project) : project(project) {}

	virtual void start(ZoomableDrawingArea* canvas) {
		Creator::start(canvas);
		PromScript* s = new PromScript();
		s->name = "New script";

		PromGroup* f_debut = new PromGroup(s);
		f_debut->group = "f_debut";
		s->add_group(f_debut);

		if(!project->net) { PromNet* net = new PromNet(); project->load_net(net);}
		project->net->add(new PromNode(project->net, s));
		project->add(s);

		script = project->get(s);
		dummy_group = project->get(f_debut);
	}

	virtual void create(double x, double y) {
		bKeep = true;
		end();
	}

	virtual void end() {
		if(!bKeep) delete script;
		else (new CommandPromScriptCreate(project,script))->execute();
		Creator::end();
		Workbench::cur()->update();
	}

	virtual void render(Graphics& g) {}

	virtual void on_mouse_move(GdkEventMotion* e) {
		Creator::on_mouse_move(e);
		dummy_group->component->center(Vector2D(x,y));
	}
};


#endif /* PROMSCRIPTCREATOR_H_ */
