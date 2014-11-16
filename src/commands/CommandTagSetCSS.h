/*
 * CommandTagSetCSS.h
 *
 *  Created on: 14 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandTagSetCSS_H_
#define CommandTagSetCSS_H_

#include <commands/Command.h>
#include <module/Document.h>
#include "../style/Tags.h"


class CommandTagSetCSS : public Command {
public:
	std::string tagname;
	std::string css;
	std::string oldcss;
	Tag* old_unique_tag = NULL;

public:
	CommandTagSetCSS(const std::string& tagname, const std::string& css) : tagname(tagname), css(css) {

	}

	virtual ~CommandTagSetCSS() {

	}

	virtual void execute() {
		Tag* t = Tags::get(tagname);
		if(!t) {ERROR("No such tag : " << tagname); return;}
		old_unique_tag = Tags::get_by_class(css);
		if(old_unique_tag) old_unique_tag->set_css_class("");
		oldcss = t->css_class;
		t->set_css_class(css);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual void undo() {
		Tag* t = Tags::get(tagname);
		if(!t) {ERROR("No such tag : " << tagname); return;}
		if(old_unique_tag) old_unique_tag->set_css_class(css);
		t->set_css_class(oldcss);
		Document::cur()->fire_change_event();
		ZoomableDrawingArea::cur()->repaint();
	}

	virtual std::string get_text() {
		return TOSTRING("Style Tag " << css);
	}
};

#endif /* CommandTagSetCSS_H_ */
