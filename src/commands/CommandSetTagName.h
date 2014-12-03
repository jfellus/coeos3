/*
 * CommandSetTagName.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandSetTagName_H_
#define CommandSetTagName_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


class CommandSetTagName : public Command {
public:
	Tag* tag = NULL;
	std::string newname;
	std::string oldname;

public:
	CommandSetTagName(Tag* t, const std::string& newname) : tag(t), newname(newname) {
		oldname = t->name;
	}

	virtual ~CommandSetTagName() {

	}

	virtual void execute() {
		tag->name = newname;
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		tag->name = oldname;
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return TOSTRING("Rename Tag");
	}
};

#endif /* CommandSetTagName_H_ */
