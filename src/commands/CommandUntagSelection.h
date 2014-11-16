/*
 * CommandUntagSelection.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandUntagSelection_H_
#define CommandUntagSelection_H_

#include <commands/Command.h>
#include <module/Document.h>
#include <module/Module.h>
#include <module/Link.h>
#include "../style/Tags.h"


class CommandUntagSelection : public Command {
public:
	std::string tagname;
	std::vector<Module*> modules;
	std::vector<Link*> links;
public:
	CommandUntagSelection(Document* doc, const std::string& tagname) : tagname(tagname), modules(doc->selected_modules), links(doc->selected_links) {

	}

	virtual ~CommandUntagSelection() {

	}

	virtual void execute() {
		Tag* t = Tags::get(tagname);
		if(!t) {ERROR("No such tag : " << tagname); return;}
		for(uint i=0; i<modules.size(); i++) t->remove(modules[i]);
		for(uint i=0; i<links.size(); i++) t->remove(links[i]);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		Tag* t = Tags::get(tagname);
		if(!t) {ERROR("No such tag : " << tagname); return;}
		for(uint i=0; i<modules.size(); i++) t->add(modules[i]);
		for(uint i=0; i<links.size(); i++) t->add(links[i]);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return "Untag selection";
	}
};

#endif /* CommandUntagSelection_H_ */
