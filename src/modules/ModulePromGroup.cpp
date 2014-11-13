/*
 * ModulePromGroup.cpp
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#include "ModulePromGroup.h"
#include "../promethe/PromProject.h"


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




std::ostream& operator<<(std::ostream& os, ModulePromGroup* a) {
	a->dump(os);
	return os;
}

void ModulePromGroup::detach(bool bSlave) {
	if(!bAttached) return;
	Module::detach(bSlave);
	if(group->script) group->script->remove_group(this->group);
}

void ModulePromGroup::attach() {
	if(bAttached) return;
	Module::attach();
	if(group->script) group->script->add_group(this->group);
}
