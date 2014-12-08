/*
 * CommandScriptSetProperty.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandScriptSetProperty_H_
#define CommandScriptSetProperty_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../promethe/promnet/PromNode.h"




using namespace libboiboites;
namespace coeos {



class CommandScriptSetProperty : public Command {
public:
	PromNode* node;
	std::string key;
	std::string value;
	std::string oldvalue;
public:
	CommandScriptSetProperty(PromNode* node, const std::string& key, const std::string& value) : node(node), key(key), value(value) {
		oldvalue = node->properties.get_as_string(key);
	}

	virtual ~CommandScriptSetProperty() {

	}

	virtual void execute() {
		node->set_property(key, value);
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		node->set_property(key, oldvalue);
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return TOSTRING("Script set " << key);
	}
};



}


#endif /* CommandScriptSetProperty_H_ */
