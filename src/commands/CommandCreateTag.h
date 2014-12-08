/*
 * CommandCreateTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandCreateTag_H_
#define CommandCreateTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"

using namespace libboiboites;
namespace coeos {



class CommandCreateTag : public Command {
public:
	std::string tagname;
	Tag* tag = NULL;
public:
	CommandCreateTag(const std::string& tagname) : tagname(tagname) {
		tag = new Tag(tagname);
	}

	virtual ~CommandCreateTag() {

	}

	virtual void execute() {
		Tags::add(tag);
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		Tags::remove(tag);
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return TOSTRING("Create Tag " << tagname);
	}
};


}

#endif /* CommandCreateTag_H_ */
