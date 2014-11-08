/*
 * GroupPromScript.h
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#ifndef GROUPPROMSCRIPT_H_
#define GROUPPROMSCRIPT_H_


#include <module/Group.h>
#include "../promethe/promscript/PromScript.h"

class GroupPromScript : public Group {
public:
	PromScript* script = NULL;
	PromProject* project = NULL;
	PromNode* node = NULL;
public:
	GroupPromScript(PromScript* script) : script(script) {
		project = script->project;
		node = script->node;
	}

	virtual void realize();
};



#endif /* GROUPPROMSCRIPT_H_ */
