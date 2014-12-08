/*
 * DocBrowser.h
 *
 *  Created on: 24 oct. 2014
 *      Author: jfellus
 */

#ifndef DOCBROWSER_H_
#define DOCBROWSER_H_

#include <widget/Browser.h>
#include "../../modules/ModulePromGroup.h"
#include "../../promethe/promscript/PromGroup.h"


using namespace libboiboites;
namespace coeos {



class DocBrowser : public Browser {
public:
	PromGroup* selectedGroup = 0;
	int nbSelected = 0;
public:
	DocBrowser();
	virtual ~DocBrowser();

	void update(std::vector<Module*>* selection) {
		nbSelected = 0;
		for(uint i=0; i<selection->size(); i++) {
			ModulePromGroup* mpg = dynamic_cast<ModulePromGroup*>((*selection)[i]);
			if(mpg) {
				selectedGroup = mpg->group;
				nbSelected++;
			}
		}
		if(nbSelected!=1) selectedGroup = 0;
		update();
	}

	virtual std::string answer(const std::string& request, const std::string& data) {
		if(selectedGroup==NULL) return TOSTRING("MULTIPLE_SELECTION:" << nbSelected);
		return TOSTRING("{ " <<
				JSON_P("type", selectedGroup->get_type()) << ", " <<
				JSON_P("name", selectedGroup->get_name()) << ", " <<
				JSON_P("author", "un ami") <<
		 " }");
	}

	std::string get_doc_page(PromGroup* g) {
		std::string page = str_to_lower(g->get_type());
		std::string s = TOSTRING("src/js/doc/" << page << ".html");
		if(file_exists(s)) return s;
		else return "src/js/doc/_default.html";
	}

	virtual void do_update() {
		if(selectedGroup==NULL) open("src/js/doc/_multiple.html");
		else open(get_doc_page(selectedGroup));
	}

	virtual void update() { Browser::update(); }
};


}

#endif /* DOCBROWSER_H_ */

