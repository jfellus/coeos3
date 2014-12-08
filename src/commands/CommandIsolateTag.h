/*
 * CommandIsolateTag.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandIsolateTag_H_
#define CommandIsolateTag_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


using namespace libboiboites;
namespace coeos {


class CommandIsolateTag : public Command {
public:
	std::string tagname;
	std::string oldtagname;

public:
	CommandIsolateTag(Document* doc, const std::string& tagname) : tagname(tagname) {

	}

	virtual ~CommandIsolateTag() {

	}

	virtual void execute() {
		if(!Tags::get(tagname)) {ERROR("No such tag : " << tagname); return;}
		if(Tags::isolated) oldtagname = Tags::isolated->name;
		Tags::isolate(tagname);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		Tags::unisolate();
		if(!oldtagname.empty()) Tags::isolate(oldtagname);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Isolate " << tagname);
	}
};



}

#endif /* CommandIsolateTag_H_ */
