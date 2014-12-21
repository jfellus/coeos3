/*
 * LinkPromLink.cpp
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */


#include "LinkPromLink.h"
#include "../promethe/PromProject.h"
#include <components/LinkLinkComponent.h>

using namespace libboiboites;
namespace coeos {




LinkPromLink::~LinkPromLink() { link->project->remove(this); delete link; }


void LinkPromLink::realize() {
	ModulePromGroup* s = link->project->get(link->src);
	if(!s) { ERROR("Unknown module : " << link->src); return;}
	ModulePromGroup* d = link->project->get(link->dst);
	if(!d) { ERROR("Unknown module : " << link->dst); return;}
	connect(s,d);
	if(link->is_type_algo()) add_class("algo");
	else {
		if(link->type == No_l_1_a || link->type == No_l_1_t) add_class("one_to_all");
		if(link->type == No_l_1_1_modif || link->type == No_l_1_1_non_modif) add_class("one_to_one");
		if(link->type == No_l_neuro_mod) add_class("neuromod");
	}

	if(link->secondary > 0) add_class("secondary");

	if(link->annotations.get("bezier_x2")) component->b->x2 = TOFLOAT(link->annotations.get_as_string("bezier_x2"));
	if(link->annotations.get("bezier_y2")) component->b->y2 = TOFLOAT(link->annotations.get_as_string("bezier_y2"));
	if(link->annotations.get("bezier_x3")) component->b->x3 = TOFLOAT(link->annotations.get_as_string("bezier_x3"));
	if(link->annotations.get("bezier_y3")) component->b->y3 = TOFLOAT(link->annotations.get_as_string("bezier_y3"));
	if(!link->annotations.get("bezier_x2") && src==dst) component->b->set(1200,0,0,-1200);
}

void LinkPromLink::connect(ModulePromGroup* src, ModulePromGroup* dst) {
	if(!src || !dst) { ERROR("Connection error"); return; }
	Link::connect(src,dst);
	if(link) {
		link->src = src->group;
		link->dst = dst->group;

		if(!link->annotations.get("bezier_x2") && src==dst) component->b->set(1200,0,0,-1200);
	}
}

void LinkPromLink::connect(Module* src, Module* dst) {
	if(dynamic_cast<ModulePromGroup*>(src) && dynamic_cast<ModulePromGroup*>(dst))
		connect(dynamic_cast<ModulePromGroup*>(src),dynamic_cast<ModulePromGroup*>(dst));
	else Link::connect(src,dst);
}

std::ostream& operator<<(std::ostream& os, LinkPromLink* a) {
	a->dump(os);
	return os;
}


Link* LinkPromLink::copy() {
	LinkPromLink* l = new LinkPromLink(link->copy());
	return l;
}

void LinkPromLink::detach(bool bSlave) {
	if(!bAttached) return;
	Link::detach(bSlave);
	if(link->script) link->script->remove_link(this->link);
}

void LinkPromLink::attach() {
	if(bAttached) return;
	Link::attach();
	if(link->script) link->script->add_link(this->link);
}

void LinkPromLink::scale(float amount) { ((LinkLinkComponent*)component)->scale(amount);}


void LinkPromLink::on_change() {
	link->annotations.set("bezier_x2", TOSTRING(component->b->x2));
	link->annotations.set("bezier_y2", TOSTRING(component->b->y2));
	link->annotations.set("bezier_x3", TOSTRING(component->b->x3));
	link->annotations.set("bezier_y3", TOSTRING(component->b->y3));
}


}
