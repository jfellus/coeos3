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


using namespace libboiboites;
namespace coeos {


class PromGroupCreator : public ModuleCreator {
public:
	PromProject* project;
	std::string nametype;
public:
	PromGroupCreator(PromProject* project);
	PromGroupCreator(PromProject* project, const std::string& nametype);
	virtual ~PromGroupCreator() {}


	virtual void create(double x, double y) {
		GroupPromScript* gps = PromWorkbench::cur()->get_script_at(x,y);
		if(!gps) return;
		PromGroup* promGroup = new PromGroup(gps->script, nametype);
		promGroup->posx = (int)(x);
		promGroup->posy = (int)(y);

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


}


#endif /* PROMGROUPCREATOR_H_ */
