/*
 * PromProject.h
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMPROJECT_H_
#define PROMPROJECT_H_

#include <creators/ModuleCreator.h>
#include <creators/LinkCreator.h>
#include "../workbench/PromWorkbench.h"
#include "promnet/PromNet.h"
#include "../modules/ModulePromGroup.h"
#include "../modules/LinkPromLink.h"
#include "../modules/GroupPromScript.h"


class PromProject {
public:
	std::vector<ModulePromGroup*> groups;
	std::vector<LinkPromLink*> links;
	std::vector<GroupPromScript*> scripts;
	PromNet* net = NULL;

public:
	PromProject() {}
	~PromProject() {
		while(links.size()>0) delete links[0];
		while(groups.size()>0) delete groups[0];
		if(net) delete net;
	}

	void load_net(PromNet* net);
	void save_net(const std::string& filename);
	void save_script(PromScript* script);

	void add(PromScript* script);

	inline void add(GroupPromScript* s) {scripts.push_back(s);}
	inline void add(ModulePromGroup* g) {groups.push_back(g);}
	inline void add(LinkPromLink* l) {links.push_back(l);}
	inline void remove(ModulePromGroup* g) {vector_remove(groups, g);}
	inline void remove(LinkPromLink* l) {vector_remove(links, l);}
	inline void remove(GroupPromScript* s) {vector_remove(scripts, s);}

	ModulePromGroup* get(PromGroup* g);
	ModulePromGroup* get_group_by_no_name(const std::string& s);

	LinkPromLink* get(PromLink* l);
	GroupPromScript* get(PromScript* s);

	uint get_scripts_count() { return scripts.size(); }
	uint get_modules_count() { return groups.size(); }
	uint get_links_count() { return links.size(); }

	void create_timescales_groups(PromScript* script);
	void layout_scripts();
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
		PromGroup* promGroup = new PromGroup(project);
		promGroup->type = 14;
		promGroup->group = "F_new_group";
		promGroup->posx = (uint)x;
		promGroup->posy = (uint)y;

		ModulePromGroup* m = new ModulePromGroup(promGroup);
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
			if(!c) return;
			Module* m = (Module*) c->get_user_data("Module");
			if(!m) return;
			ModulePromGroup* g = dynamic_cast<ModulePromGroup*>(m);
			if(!g) return;

			src = g;
		} else {
			Component* c = canvas->get_selectable_component_at(x,y);
			if(!c) return;
			Module* m = (Module*) c->get_user_data("Module");
			if(!m) return;
			ModulePromGroup* dst = dynamic_cast<ModulePromGroup*>(m);
			if(!dst) return;
			this->dst = dst;

			PromLink* promLink = new PromLink(project);
			promLink->src = dynamic_cast<ModulePromGroup*>(src)->group;
			promLink->dst = dst->group;
			promLink->type = 5;

			LinkPromLink* link = new LinkPromLink(promLink);
			project->add(link);
			this->link = link;

			end();
		}
	}
};

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
		Creator::end();
		Workbench::cur()->update();
	}

	virtual void render(Graphics& g) {}

	virtual void on_mouse_move(GdkEventMotion* e) {
		Creator::on_mouse_move(e);
		dummy_group->component->center(Vector2D(x,y));
	}
};

#endif /* PROMPROJECT_H_ */
