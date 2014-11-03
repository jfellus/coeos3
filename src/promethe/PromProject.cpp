/*
 * PromProject.cpp
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */

#include "PromProject.h"


void PromProject::create_timescales_groups(PromScript* script) {
	int cur_timescale = script->get_highest_timescale();
	uint done_groups = 0;
	Group* curgroup = new Group(), *last_group = 0;
	curgroup->set_property("timescale", "yes");

	while(done_groups < script->groups.size()) {
		for(uint i=0; i<groups.size(); i++) {
			if(groups[i]->group->script !=  script) continue;
			PromGroup* g = groups[i]->group;
			if(g->time_scale == cur_timescale) {
				if(curgroup) curgroup->add(groups[i]);
				cur_timescale = g->time_scale;
				done_groups++;
			}
		}
		if(curgroup) curgroup->text = TOSTRING("time_scale " << cur_timescale);
		if(last_group && curgroup) {last_group->add(curgroup); curgroup->open(); last_group->open();}
		last_group = curgroup;

		if(done_groups >= script->groups.size()) break;

		curgroup = new Group();
		curgroup->set_property("timescale", "yes");
		cur_timescale--;
	}
}


void PromProject::save_to_single_script(const std::string& filename) {
	PromScript* single_script = loaded_scripts.empty() ? new PromScript() : loaded_scripts[0];
	int max_timescale = 0;

	// Assign time scales add attach to single script
	for(uint i=0; i<groups.size(); i++) {
		PromGroup* g = groups[i]->group;
		if(g->script!=single_script) single_script->add_group(g);
		g->time_scale = infer_timescale(groups[i]->parent);
		if(g->time_scale > max_timescale) max_timescale = g->time_scale;
	}
	for(uint i=0; i<links.size(); i++) {
		PromLink* l = links[i]->link;
		if(l->script!=single_script) single_script->add_link(l);
	}

	// Reverse time scales indices
	for(uint i=0; i<groups.size(); i++) groups[i]->group->time_scale = max_timescale - groups[i]->group->time_scale;

	single_script->save(filename);
}

int PromProject::infer_timescale(Group* g) {
	int ts = 0;
	Group* p = g;
	while(p!=NULL) {
		if(g->properties.get("timescale") && g->properties.get("timescale")->get_value_as_string()=="yes") ts++;
		p = p->parent;
	}
	return ts;
}
