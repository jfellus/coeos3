/*
 * CommandPromGroupCreate.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef COMMANDPROMGROUPCREATE_H_
#define COMMANDPROMGROUPCREATE_H_

#include <commands/Command.h>
#include "../promethe/PromProject.h"

class CommandPromGroupCreate : public Command {
public:
	PromProject* project;
	ModulePromGroup* m;
	bool bFirstTime = true;
public:
	CommandPromGroupCreate(PromProject* project, ModulePromGroup* m) : project(project), m(m) {}
	virtual ~CommandPromGroupCreate() {}


	virtual void execute() {
		project->add(m);
		if(!bFirstTime) m->attach();
		bFirstTime = false;
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		project->remove(m);
		m->detach();
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return "Create module";
	}
};

#endif /* COMMANDPROMGROUPCREATE_H_ */
