/*
 * promethe_modules.cpp
 *
 *  Created on: 20 oct. 2014
 *      Author: jfellus
 */

#include "promethe_modules.h"
#include "../module/Group.h"
#include "PromProject.h"


void LinkPromLink::realize() {
	ModulePromGroup* s = link->project->get(link->src);
	if(!s) { ERROR("Unknown module : " << link->src); return;}
	ModulePromGroup* d = link->project->get(link->dst);
	if(!d) { ERROR("Unknown module : " << link->dst); return;}
	connect(s,d);
	if(link->is_type_algo()) {
		component->add_class("algo");
	} else {
		if(link->type == No_l_1_a || link->type == No_l_1_t) component->add_class("one_to_all");
		if(link->type == No_l_1_1_modif || link->type == No_l_1_1_non_modif) component->add_class("one_to_one");
		if(link->type == No_l_neuro_mod) component->add_class("neuromod");
	}

	if(link->secondary > 0) { dynamic_cast<LinkLinkComponent*>(component)->dashed = true;}
}

void ModulePromGroup::realize() {
	if(group->type==14) {
		if(!group->custom_function.empty()) {
			try { create_component(group->custom_function.c_str()); }
			catch(...) {
				try { create_component(group->group.c_str()); }
				catch(...) { create_component("module_algo"); }
			}
		} else {
			try { create_component(group->group.c_str()); }
			catch(...) { create_component("module_algo"); }
		}
	} else {
		try { create_component(group->get_type_as_string().c_str()); }
		catch(...) { create_component("module_neural"); }
	}
	component->set_pos(group->posx*10, group->posy*10);
	component->add_class(group->is_type_algo() ? "algo" : "neural");
	if(!group->custom_function.empty()) component->add_class("custom_cpp");
}

ModulePromGroup::~ModulePromGroup() { group->project->remove(this); delete group; }
LinkPromLink::~LinkPromLink() { link->project->remove(this); delete link; }


std::ostream& operator<<(std::ostream& os, ModulePromGroup* a) {
	a->dump(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, LinkPromLink* a) {
	a->dump(os);
	return os;
}
