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

	inline std::string str_encode_doxygen(const std::string& groupname) {
		std::string s = str_replace(groupname, "_", "__");
		std::ostringstream oss;
		for(uint i=0; i<s.length(); i++) {
			if(isupper(s[i])) oss << "_" << (char) tolower(s[i]);
			else oss << s[i];
		}
		return str_replace(oss.str(), "\n", "");
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
		std::string s = TOSTRING(home() << "/bin_leto_prom/simulator/doc/doxygen/html/" << g->get_type() << ".html");
		if(file_exists(s)) return s;
		s = TOSTRING(home() << "/bin_leto_prom/simulator/doc/doxygen/html/group__" << str_encode_doxygen(g->get_type()) << ".html");
		DBGV(s);
		if(file_exists(s)) return s;
		s = TOSTRING(home() << "/bin_leto_prom/simulator/doc/doxygen/html/" << str_encode_doxygen(g->get_type()) << ".html");
		if(file_exists(s)) return s;

		return "src/js/doc/_default.html";
//		std::string page = str_to_lower(g->get_type());
//		std::string s = TOSTRING("src/js/doc/" << page << ".html");
//		if(file_exists(s)) return s;
//		else return ".coeos++/src/js/doc/_default.html";
	}

	virtual void do_update() {
		if(selectedGroup==NULL) open("src/js/doc/_multiple.html");
		else {
			open(get_doc_page(selectedGroup));
			show_only_main_frame();
		}
	}

	virtual void update() { Browser::update(); }

private:
	void show_only_main_frame();
};


}

#endif /* DOCBROWSER_H_ */

