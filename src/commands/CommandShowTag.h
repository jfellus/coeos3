/*
 * CommandShowTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandShowTag_H_
#define CommandShowTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


using namespace libboiboites;
namespace coeos {




class CommandShowTag : public Command {
public:
	std::string tagname;
	Tag* tag = NULL;
public:
	CommandShowTag(const std::string& tagname) : tagname(tagname) {
		tag = Tags::get(tagname);
	}

	virtual ~CommandShowTag() {

	}

	virtual void execute() {
		tag->show();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		tag->hide();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Show tag " << tagname);
	}
};


}


#endif /* CommandShowTag_H_ */
