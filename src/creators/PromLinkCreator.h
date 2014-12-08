/*
 * PromLinkCreator.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef PROMLINKCREATOR_H_
#define PROMLINKCREATOR_H_


#include <creator/Creator.h>
#include "../promethe/PromProject.h"
#include "../commands/CommandPromLinkCreate.h"


using namespace libboiboites;
namespace coeos {



class PromLinkCreator : public LinkCreator {
public:
	PromProject* project;
	int type;
public:
	PromLinkCreator(PromProject* project, int type = No_l_algorithmique) : LinkCreator(), project(project),type(type) {}
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

			PromLink* promLink = new PromLink(project, type);
			promLink->src = dynamic_cast<ModulePromGroup*>(src)->group;
			promLink->dst = dst->group;

			if(promLink->src->script != promLink->dst->script) {
				ERROR("Cross-script links not really handled yet... sorry");
			} else promLink->src->script->add_link(promLink);

			LinkPromLink* link = new LinkPromLink(promLink);
			(new CommandPromLinkCreate(project,link))->execute();
			this->link = link;

			end();
		}
	}
};


}



#endif /* PROMLINKCREATOR_H_ */
