/*
 * CommandHideTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandHideTag_H_
#define CommandHideTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


class CommandHideTag : public Command {
public:
	std::string tagname;
	Tag* tag = NULL;
public:
	CommandHideTag(const std::string& tagname) : tagname(tagname) {
		tag = Tags::get(tagname);
	}

	virtual ~CommandHideTag() {

	}

	virtual void execute() {
		tag->hide();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		tag->show();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Hide tag " << tagname);
	}
};

#endif /* CommandHideTag_H_ */
