/*
 * GroupPromScript.cpp
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */


#include "GroupPromScript.h"
#include <module/GroupComponent.h>


void GroupPromScript::realize() {
	create_component("script_closed");
	component->hide();
	component->add_class("script");

	component_open = new GroupOpenComponent(this);
	component_open->add_class("script");
	component_open->add_selection_listener(this);
	component_open->layer = -0.5;
}
