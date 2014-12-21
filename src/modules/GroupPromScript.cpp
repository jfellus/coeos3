/*
 * GroupPromScript.cpp
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */


#include "GroupPromScript.h"
#include <components/GroupComponent.h>
#include "../promethe/PromProject.h"
#include "../workbench/PromWorkbench.h"



using namespace libboiboites;
namespace coeos {



GroupPromScript::GroupPromScript(PromScript* script) : script(script) {
	project = script->project;
	node = script->node;
	node->add_properties_listener(this);
	set_property("name", node->get_property("name"));
	properties.set("embedded", node->bEmbedded);
}


void GroupPromScript::realize() {
	create_component("script_closed");
	component->hide();
	component->add_class("script");

	component_open = new GroupOpenComponent(this);
	component_open->add_class("script");
	component_open->add_selection_listener(this);
	component_open->layer = -0.5;
}


GroupPromScript::~GroupPromScript() {
	project->remove(this);
	delete script;
}

void GroupPromScript::detach(bool bSlave) {
	if(!bAttached) return;
	Group::detach(bSlave);
	if(project->net) {
		project->net->remove(node);
	}
}

void GroupPromScript::attach() {
	if(bAttached) return;
	Group::attach();
	if(project->net) {
		project->net->add(node);
	}
}



}
