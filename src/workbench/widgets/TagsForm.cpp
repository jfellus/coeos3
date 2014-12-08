/*
 * TagsForm.cpp
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#include "TagsForm.h"
#include "../../promethe/library/ModulesLibrary.h"
#include "../../workbench/PromWorkbench.h"
#include "../../style/Tags.h"

using namespace libboiboites;
namespace coeos {


std::string TagsForm::answer(const std::string& request, const std::string& data) {
	if(str_starts_with(request, "isolate/")) {
		PromWorkbench::cur()->isolate_tag(request.substr(strlen("isolate/")));
	}
	else if(str_starts_with(request, "create_for_selection")) {
		std::string tn = Tags::new_tagname();
		PromWorkbench::cur()->add_tag(tn);
		PromWorkbench::cur()->tag_selection(tn);
	}
	else if(str_starts_with(request, "create")) {
		std::string tn = Tags::new_tagname();
		PromWorkbench::cur()->add_tag(tn);
	} else if(str_starts_with(request, "show/")) {
		std::string tn = request.substr(strlen("show/"));
		PromWorkbench::cur()->show_tag(tn);
	} else if(str_starts_with(request, "hide/")) {
		std::string tn = request.substr(strlen("hide/"));
		PromWorkbench::cur()->hide_tag(tn);
	} else if(str_starts_with(request, "toggle_isolate/")) {
		std::string tn = request.substr(strlen("toggle_isolate/"));
		PromWorkbench::cur()->isolate_tag(tn);
	} else if(str_starts_with(request, "lock/")) {
		std::string tn = request.substr(strlen("lock/"));
		PromWorkbench::cur()->lock_tag(tn);
	} else if(str_starts_with(request, "unlock/")) {
		std::string tn = request.substr(strlen("unlock/"));
		PromWorkbench::cur()->unlock_tag(tn);
	} else if(str_starts_with(request, "add_selected/")) {
		std::string tn = request.substr(strlen("add_selected/"));
		PromWorkbench::cur()->tag_selection(tn);
	} else if(str_starts_with(request, "remove_selected/")) {
		std::string tn = request.substr(strlen("remove_selected/"));
		PromWorkbench::cur()->untag_selection(tn);
	} else if(str_starts_with(request, "set_css/")) {
		std::string s = request.substr(strlen("set_css/"));
		std::string tagcol = "tag_" +s.substr(0,s.find('/'));
		std::string tn = s.substr(s.find('/')+1);
		PromWorkbench::cur()->tag_set_css(tn, tagcol);
	} else if(str_starts_with(request, "unset_css/")) {
		std::string s = request.substr(strlen("unset_css/"));
		std::string tagcol = "tag_" +s.substr(0,s.find('/'));
		std::string tn = s.substr(s.find('/')+1);
		PromWorkbench::cur()->tag_set_css(tn, "");
	} else if(str_starts_with(request, "set_name/")) {
		std::string s = request.substr(strlen("set_name/"));
		std::string tn = s.substr(0,s.find('/'));
		std::string newname = s.substr(s.find('/')+1);
		PromWorkbench::cur()->change_tag_name(tn, newname);
	}
	else {
		std::string ans = "[ ";
		Properties p; p.set("nb_selected", PromWorkbench::cur()->get_selected_modules_count() + PromWorkbench::cur()->get_selected_links_count());
		ans += p.to_JSON();
		for(uint i=0; i<Tags::tags.size(); i++) {
			ans += ", ";
			Tag* t = Tags::tags[i];
			Properties p;
			p.set("name", &t->name);
			p.set("nb_modules", t->modules.size());
			p.set("nb_links", t->links.size());
			p.set("class", &t->css_class);
			p.set("isolated", t == Tags::isolated);
			p.set("visible", &t->bVisible);
			p.set("lock", &t->bLock);
			ans += p.to_JSON();
		}
		ans += " ]";
		return ans;
	}
	return "ok";
}

}
