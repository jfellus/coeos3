/*
 * CommandPromLinkCreate.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef COMMANDPROMLINKCREATE_H_
#define COMMANDPROMLINKCREATE_H_


#include <commands/Command.h>
#include "../promethe/PromProject.h"



using namespace libboiboites;
namespace coeos {


class CommandPromLinkCreate : public Command {
public:
	PromProject* project;
	LinkPromLink* l;
	bool bFirstTime = true;
public:
	CommandPromLinkCreate(PromProject* project, LinkPromLink* l) : project(project), l(l) {}
	virtual ~CommandPromLinkCreate() {}

	virtual void execute() {
		project->add(l);
		if(!bFirstTime) l->attach();
		bFirstTime = false;
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		project->remove(l);
		l->detach();
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return "Create link";
	}
};


}

#endif /* COMMANDPROMLINKCREATE_H_ */
