/*
 * PromScriptCreator.cpp
 *
 *  Created on: 15 nov. 2014
 *      Author: jfellus
 */



#include "PromScriptCreator.h"


using namespace libboiboites;
namespace coeos {


static std::string new_name(PromNet* net) {
	static uint z = 1;
	if(!net) return TOSTRING("New Script " << z++);
	for(uint i = 1; ;i++) {
		std::string s = TOSTRING("New Script " << i);
		if(!net->get_script_by_name(s))	return s;
	}
	return "";
}


void PromScriptCreator::start(ZoomableDrawingArea* canvas) {
	canvas->OFF();
	Creator::start(canvas);
	if(!project->net) project->set_net(new PromNet());

	PromScript* s = new PromScript();
	s->name = new_name(project->net);
	PromGroup* f_debut = new PromGroup(s, "f_debut");
	s->add_group(f_debut);

	project->net->add(new PromNode(project->net, s));
	project->add(s);

	script = project->get(s);
	dummy_group = project->get(f_debut);
	dummy_group->component->center(Vector2D(x,y));

	Document::cur()->update_links_layers();
	canvas->update_layers();
	canvas->ON();
	Workbench::cur()->update();
}

void PromScriptCreator::create(double x, double y) {
	bKeep = true;
	dummy_group->set_property("posx", TOSTRING((int)(x)));
	dummy_group->set_property("posy", TOSTRING((int)(y)));
	end();
}

void PromScriptCreator::end() {
	if(!bKeep) delete script;
	else (new CommandPromScriptCreate(project,script))->execute();
	Creator::end();
	Workbench::cur()->update();
}


}
