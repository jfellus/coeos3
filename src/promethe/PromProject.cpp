/*
 * PromProject.cpp
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */

#include "PromProject.h"


void PromProject::create_timescales_groups(PromScript* script) {
	int max_timescale = script->get_highest_timescale();
	int cur_timescale = max_timescale;
	uint done_groups = 0;
	GroupPromScript* gps = new GroupPromScript(script);
	gps->realize();
	add(gps);
	gps->set_property("name", script->name);
	Group* curgroup = new Group(), *last_group = 0;
	curgroup->realize();
	gps->add(curgroup);
	curgroup->open(); gps->open(); gps->unselect();
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
		if(last_group && curgroup) {
			last_group->add(curgroup); curgroup->open(); last_group->open();
			last_group->unselect(); curgroup->unselect();
		}
		last_group = curgroup;

		if(done_groups >= script->groups.size()) break;

		curgroup = new Group();
		curgroup->realize();
		curgroup->set_property("timescale", "yes");
		cur_timescale--;
	}

}

void PromProject::layout_scripts() {
	Rectangle r;
	double W=0,H=0;
	for(uint i = 0; i<Document::cur()->modules.size(); i++) {
		GroupPromScript* gps = dynamic_cast<GroupPromScript*>(Document::cur()->modules[i]);
		if(!gps) continue;
		W += gps->get_bounds().w;
		H = MAX(r.h, gps->get_bounds().h);
	}
	float ratio = W/H + 1; W /= sqrt(ratio);
	r.x = r.y = r.w = r.h = 0;
	for(uint i = 0; i<Document::cur()->modules.size(); i++) {
		GroupPromScript* gps = dynamic_cast<GroupPromScript*>(Document::cur()->modules[i]);
		if(!gps) continue;
		Rectangle r2 = gps->get_bounds();
		gps->translate(r.x - r2.x, r.y - r2.y);
		r.w = r2.w + 700;
		r.h = MAX(r.h, r2.h);
		r.x += r.w;
		if(r.x > W) {r.x = 0; r.y += r.h + 700; r.h = 0;}
	}
	for(uint i = 0; i<Document::cur()->modules.size(); i++) {
			GroupPromScript* gps = dynamic_cast<GroupPromScript*>(Document::cur()->modules[i]);
			if(!gps) continue;
			gps->close();
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

GroupPromScript* PromProject::get(PromScript* s) {
	for(uint i=0; i<scripts.size(); i++) {
		if(scripts[i]->script==s) return scripts[i];
	}
	return NULL;
}

LinkPromLink* PromProject::get(PromLink* l) {
	for(uint i=0; i<links.size(); i++) {
		if(links[i]->link==l) return links[i];
	}
	return NULL;
}

ModulePromGroup* PromProject::get(PromGroup* g) {
	for(uint i=0; i<groups.size(); i++) {
		if(groups[i]->group==g) return groups[i];
	}
	return NULL;
}

ModulePromGroup* PromProject::get_group_by_no_name(const std::string& s) {
	for(uint i=0; i<groups.size(); i++) {
		if(groups[i]->group->no_name==s) return groups[i];
	}
	return NULL;
}

void PromProject::add(PromScript* script) {
	STATUS("Loading " << script->filename << " (" << script->groups.size() << " groups, " << script->links.size() << " links)");
	if(!net) {	net = new PromNet(); net->project = this; }
	script->project = this;
	for(uint i=0; i<script->groups.size(); i++) {
		script->groups[i]->project = this;
		add(new ModulePromGroup(script->groups[i]));
	}
	for(uint i=0; i<script->links.size(); i++) {
		script->links[i]->project = this;
		add(new LinkPromLink(script->links[i]));
	}
	create_timescales_groups(script);
}

void PromProject::load_net(PromNet* net) {
	if(this->net) delete this->net;
	this->net = net;
	net->project = this;
	try {
		net->realize();
		for(uint i=0; i<net->nodes.size(); i++) add(net->nodes[i]->script);
	} catch(...) {ERROR("Can't open network " << net); throw "";}
}

