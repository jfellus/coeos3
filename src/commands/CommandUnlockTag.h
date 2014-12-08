/*
 * CommandUnlockTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandUnlockTag_H_
#define CommandUnlockTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"



using namespace libboiboites;
namespace coeos {




class CommandUnlockTag : public Command {
public:
	std::string tagname;
	Tag* tag = NULL;
public:
	CommandUnlockTag(const std::string& tagname) : tagname(tagname) {
		tag = Tags::get(tagname);
	}

	virtual ~CommandUnlockTag() {

	}

	virtual void execute() {
		tag->unlock();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		tag->lock();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Unlock tag " << tagname);
	}
};



}


#endif /* CommandUnlockTag_H_ */
