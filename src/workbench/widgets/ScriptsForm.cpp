/*
 * ScriptsForm.cpp
 *
 *  Created on: 7 nov. 2014
 *      Author: jfellus
 */

#include "ScriptsForm.h"
#include "../PromWorkbench.h"
#include "../../promethe/PromProject.h"


std::string ScriptsForm::answer(const std::string& request, const std::string& data) {
	PromWorkbench* w = PromWorkbench::cur();
	if(!w->project) return "";
	std::string ans = "[ ";
	PromNet* net = w->project->net;
	for(uint i=0; PromWorkbench::cur()->project && PromWorkbench::cur()->project->net && i<net->nodes.size(); i++) {
		PromNode* n = net->nodes[i];
		if(i!=0) ans += ", ";
		ans += n->properties.to_JSON();
	}
	ans += "]";
	if(PromWorkbench::cur()->project && PromWorkbench::cur()->project->net)
	return ans;
	else return "";
}
