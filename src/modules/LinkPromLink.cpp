/*
 * LinkPromLink.cpp
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */


#include "LinkPromLink.h"
#include "../promethe/PromProject.h"
#include <components/LinkLinkComponent.h>



LinkPromLink::~LinkPromLink() { link->project->remove(this); delete link; }


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



std::ostream& operator<<(std::ostream& os, LinkPromLink* a) {
	a->dump(os);
	return os;
}

