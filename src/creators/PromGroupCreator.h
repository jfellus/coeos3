/*
 * PromGroupCreator.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef PROMGROUPCREATOR_H_
#define PROMGROUPCREATOR_H_

#include <creator/Creator.h>
#include "../promethe/PromProject.h"
#include "../commands/CommandPromGroupCreate.h"


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
		(new CommandPromGroupCreate(project,m))->execute();
		module = m;
		comp = module->component;
		comp->set_pos(x,y);
		end();
	}

	virtual void end() {
		if(dynamic_cast<ModulePromGroup*>(module) && group) {
			Group* g = group;
			while(g!=NULL) {
				GroupPromScript* s = dynamic_cast<GroupPromScript*>(g);
				if(s) {
					s->script->add_group(dynamic_cast<ModulePromGroup*>(module)->group);
					break;
				}
				g = g->parent;
			}
		}
		ModuleCreator::end();
	}
};


#endif /* PROMGROUPCREATOR_H_ */
