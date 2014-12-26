/*
 * ScriptsForm.cpp
 *
 *  Created on: 7 nov. 2014
 *      Author: jfellus
 */

#include "ScriptsForm.h"
#include "../PromWorkbench.h"
#include "../../promethe/PromProject.h"


using namespace libboiboites;
namespace coeos {



std::string ScriptsForm::answer(const std::string& request, const std::string& data) {
	PromWorkbench::cur()->LOCK();
	std::string ret = "ok";
	if(str_starts_with(request, "launch/")) {
		std::string script = request.substr(strlen("launch/"));
		PromWorkbench::cur()->launch_script(script);
	} else if(str_starts_with(request, "launch_gui/")) {
		std::string script = request.substr(strlen("launch_gui/"));
		PromWorkbench::cur()->launch_script(script, true);
	} else if(str_starts_with(request, "stop/")) {
		std::string script = request.substr(strlen("stop/"));
		PromWorkbench::cur()->stop_script(script);
	} else if(str_starts_with(request, "set/")) {
		std::string s = request.substr(strlen("set/"));
		std::string script = s.substr(0, s.find("/"));
		std::string kv = s.substr(s.find("/")+1);
		std::string key = kv.substr(0,kv.find("/"));
		std::string value = kv.substr(kv.find("/")+1);
		PromWorkbench::cur()->set_script_property(script, key, value);
	} else {
		PromWorkbench* w = PromWorkbench::cur();
		if(!w->project) ret = "";
		else {
			std::string ans = "[ ";
			PromNet* net = w->project->net;
			for(uint i=0; PromWorkbench::cur()->project && PromWorkbench::cur()->project->net && i<net->nodes.size(); i++) {
				PromNode* n = net->nodes[i];
				if(i!=0) ans += ", ";
				ans += n->properties.to_JSON();
			}
			ans += "]";
			ret = PromWorkbench::cur()->project && PromWorkbench::cur()->project->net ? ans : "";
		}
	}
	PromWorkbench::cur()->UNLOCK();
	return ret;
}


}
