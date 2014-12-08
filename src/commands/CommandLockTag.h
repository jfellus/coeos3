/*
 * CommandLockTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandLockTag_H_
#define CommandLockTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


using namespace libboiboites;
namespace coeos {


class CommandLockTag : public Command {
public:
	std::string tagname;
	Tag* tag = NULL;
public:
	CommandLockTag(const std::string& tagname) : tagname(tagname) {
		tag = Tags::get(tagname);
	}

	virtual ~CommandLockTag() {

	}

	virtual void execute() {
		tag->lock();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		tag->unlock();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Lock tag " << tagname);
	}
};


}


#endif /* CommandLockTag_H_ */
