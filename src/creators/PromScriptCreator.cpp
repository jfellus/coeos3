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
}

void PromScriptCreator::create(double x, double y) {
	bKeep = true;
	dummy_group->set_property("posx", TOSTRING((int)(x/10)));
	dummy_group->set_property("posy", TOSTRING((int)(y/10)));
	end();
}

void PromScriptCreator::end() {
	if(!bKeep) delete script;
	else (new CommandPromScriptCreate(project,script))->execute();
	Creator::end();
	Workbench::cur()->update();
}


}
