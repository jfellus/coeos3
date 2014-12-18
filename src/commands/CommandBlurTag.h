/*
 * CommandBlurTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandBlurTag_H_
#define CommandBlurTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


using namespace libboiboites;
namespace coeos {


class CommandBlurTag : public Command {
public:
	std::string tagname;
	std::string oldtagname;

public:
	CommandBlurTag(Document* doc, const std::string& tagname) : tagname(tagname) {

	}

	virtual ~CommandBlurTag() {

	}

	virtual void execute() {
		if(!Tags::get(tagname)) {ERROR("No such tag : " << tagname); return;}
		Tags::get(tagname)->blur();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		Tags::get(tagname)->unblur();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Blur " << tagname);
	}
};



}

#endif /* CommandBlurTag_H_ */
