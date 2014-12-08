/*
 * CommandPromScriptCreate.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef COMMANDPROMSCRIPTCREATE_H_
#define COMMANDPROMSCRIPTCREATE_H_


#include <commands/Command.h>
#include "../promethe/PromProject.h"



using namespace libboiboites;
namespace coeos {


class CommandPromScriptCreate : public Command {
public:
	PromProject* project;
	GroupPromScript* s;
	bool bFirstTime = true;
public:
	CommandPromScriptCreate(PromProject* project, GroupPromScript* s) : project(project), s(s) {}
	virtual ~CommandPromScriptCreate() {}

	virtual void execute() {
		if(!bFirstTime) {
			project->add(s);
			s->attach();
		}
		bFirstTime = false;
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		project->remove(s);
		s->detach();
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return "Create script";
	}
};


}

#endif /* COMMANDPROMSCRIPTCREATE_H_ */
