/*
 * PromProject.h
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMPROJECT_H_
#define PROMPROJECT_H_

#include "promethe_modules.h"
#include "PromWorkbench.h"
#include "../components/ModuleCreator.h"
#include "../components/LinkCreator.h"

class PromProject {
public:
	std::vector<ModulePromGroup*> groups;
	std::vector<LinkPromLink*> links;
	std::vector<PromScript*> loaded_scripts;

public:
	PromProject() {}
	~PromProject() {
		while(links.size()>0) delete links[0];
		while(groups.size()>0) delete groups[0];
	}

	void add(ModulePromGroup* g) {g->project = this; groups.push_back(g);}
	void add(LinkPromLink* l) {l->project = this; links.push_back(l);}

	void remove(ModulePromGroup* g) {vector_remove(groups, g);}
	void remove(LinkPromLink* l) {vector_remove(links, l);}

	ModulePromGroup* get(PromGroup* g) {
		for(uint i=0; i<groups.size(); i++) {
			if(groups[i]->group==g) return groups[i];
		}
		return NULL;
	}

	ModulePromGroup* get_group_by_name(const std::string& s) {
		for(uint i=0; i<groups.size(); i++) {
			if(groups[i]->group->name==s) return groups[i];
		}
		return NULL;
	}

	void add(PromScript* script) {
		script->project = this;
		loaded_scripts.push_back(script);
		for(uint i=0; i<script->groups.size(); i++) add(new ModulePromGroup(this, script->groups[i]));
		for(uint i=0; i<script->links.size(); i++) add(new LinkPromLink(this, script->links[i]));
		create_timescales_groups(script);
	}

	void create_timescales_groups(PromScript* script);


	void save_to_single_script(const std::string& filename);

	int infer_timescale(Group* g);
};

class PromGroupCreator : public ModuleCreator {
public:
	PromProject* project;
public:
	PromGroupCreator(PromProject* project) : ModuleCreator(), project(project) {}
	virtual ~PromGroupCreator() {}

	virtual void create(double x, double y) {
		PromGroup* promGroup = new PromGroup();
		promGroup->type = 14;
		promGroup->group = "F_new_group";
		promGroup->posx = (uint)x;
		promGroup->posy = (uint)y;

		ModulePromGroup* m = new ModulePromGroup(project, promGroup);
		project->add(m);

		module = m;
		comp = module->component;
		comp->set_pos(x,y);
		end();
	}
};


class PromLinkCreator : public LinkCreator {
public:
	PromProject* project;
public:
	PromLinkCreator(PromProject* project) : LinkCreator(), project(project) {}
	virtual ~PromLinkCreator() {}

	virtual void create(double x, double y) {
		if(!src) {
			Component* c = canvas->get_selectable_component_at(x,y);
			Module* m = (Module*) c->get_user_data("Module");
			if(!m) return;
			ModulePromGroup* g = dynamic_cast<ModulePromGroup*>(m);
			if(!g) return;

			src = g;

			repaint();
		} else {
			Component* c = canvas->get_selectable_component_at(x,y);
			if(!c) return;
			Module* m = (Module*) c->get_user_data("Module");
			if(!m) return;
			ModulePromGroup* dst = dynamic_cast<ModulePromGroup*>(m);
			if(!dst) return;
			this->dst = dst;

			PromLink* promLink = new PromLink();
			promLink->src = dynamic_cast<ModulePromGroup*>(src)->group;
			promLink->dst = dst->group;
			promLink->type = 5;

			LinkPromLink* link = new LinkPromLink(project, promLink);
			project->add(link);
			this->link = link;

			end();
		}
	}
};

#endif /* PROMPROJECT_H_ */
