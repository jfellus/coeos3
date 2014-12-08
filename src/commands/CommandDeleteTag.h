/*
 * CommandDeleteTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandDeleteTag_H_
#define CommandDeleteTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


using namespace libboiboites;
namespace coeos {


class CommandDeleteTag : public Command {
public:
	std::string tagname;
	Tag* tag = NULL;
public:
	CommandDeleteTag(const std::string& tagname) : tagname(tagname) {
		tag = Tags::get(tagname);
	}

	virtual ~CommandDeleteTag() {

	}

	virtual void execute() {
		Tags::remove(tag);
		Document::cur()->fire_change_event();
	}

	virtual void undo() {
		Tags::add(tag);
		Document::cur()->fire_change_event();
	}

	virtual std::string get_text() {
		return TOSTRING("Delete Tag " << tagname);
	}
};


}

#endif /* CommandDeleteTag_H_ */
