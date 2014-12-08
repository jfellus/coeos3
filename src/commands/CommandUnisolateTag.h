/*
 * CommandUnisolateTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandUnisolateTag_H_
#define CommandUnisolateTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"



using namespace libboiboites;
namespace coeos {





class CommandUnisolateTag : public Command {
public:
	std::string tagname;

public:
	CommandUnisolateTag() {

	}

	virtual ~CommandUnisolateTag() {

	}

	virtual void execute() {
		if(!Tags::isolated) {ERROR("No tag to unisolate "); return;}
		tagname = Tags::isolated->name;
		Tags::unisolate();
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		if(!Tags::get(tagname)) {ERROR("No such tag : " << tagname); return;}
		Tags::isolate(tagname);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Unisolate");
	}
};


}


#endif /* CommandUnisolateTag_H_ */
