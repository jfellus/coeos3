/*
 * PromProject.cpp
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */

#include "PromProject.h"
#include "promethe_modules.h"


void PromProject::create_timescales_groups(PromScript* script) {
	int max_timescale = script->get_highest_timescale();
	int cur_timescale = max_timescale;
	uint done_groups = 0;
	GroupPromScript* gps = new GroupPromScript(script);
	gps->set_property("name", script->name);
	Group* curgroup = new Group(), *last_group = 0;
	gps->add(curgroup);
	curgroup->open(); gps->open();
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
		if(curgroup) curgroup->set_property("name", TOSTRING("time_scale " << cur_timescale));
		if(last_group && curgroup) {last_group->add(curgroup); curgroup->open(); last_group->open();}
		last_group = curgroup;

		if(done_groups >= script->groups.size()) break;

		curgroup = new Group();
		curgroup->set_property("timescale", "yes");
		cur_timescale--;
	}

}

void PromProject::layout_scripts() {
	Rectangle r;
	for(uint i = 0; i<Document::cur()->modules.size(); i++) {
		GroupPromScript* gps = dynamic_cast<GroupPromScript*>(Document::cur()->modules[i]);
		if(!gps) continue;
		if(!r) r = gps->get_bounds();
		else {
			Rectangle r2 = gps->get_bounds();
			r.x += r.w + 300;
			r.w = r2.w;
			gps->translate(r.x, r.y);
		}
	}
}



void PromProject::save_to_single_script(const std::string& filename) {
	PromScript* single_script = net->nodes.empty() ? new PromScript() : net->nodes[0]->script;
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

void PromProject::save_script(PromScript* script) {
	int max_timescale = 0;

	// Assign time scales add attach to single script
	for(uint i=0; i<groups.size(); i++) {
		PromGroup* g = groups[i]->group;
		if(g->script!=script) continue;
		g->time_scale = infer_timescale(groups[i]->parent);
		if(g->time_scale > max_timescale) max_timescale = g->time_scale;
	}

	// Reverse time scales indices
	for(uint i=0; i<groups.size(); i++) {
		if(groups[i]->group->script!=script) continue;
		groups[i]->group->time_scale = max_timescale - groups[i]->group->time_scale;
	}

	script->save(script->filename.empty() ? script->name + ".symb" : script->filename);
}

void PromProject::save_net(const std::string& filename) {
	if(!net) return;

	for(uint i=0; i<net->nodes.size(); i++) {
		save_script(net->nodes[i]->script);
	}
	net->save(filename);
}
