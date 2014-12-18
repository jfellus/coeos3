/*
 * CommandUnblurTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandUnblurTag_H_
#define CommandUnblurTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


using namespace libboiboites;
namespace coeos {


class CommandUnblurTag : public Command {
public:
	std::string tagname;
	std::string oldtagname;

public:
	CommandUnblurTag(Document* doc, const std::string& tagname) : tagname(tagname) {

	}

	virtual ~CommandUnblurTag() {

	}

	virtual void execute() {
		if(!Tags::get(tagname)) {ERROR("No such tag : " << tagname); return;}
		Tags::get(tagname)->unblur();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		Tags::get(tagname)->blur();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Unblur " << tagname);
	}
};



}

#endif /* CommandUnblurTag_H_ */
