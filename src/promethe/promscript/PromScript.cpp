/*
 * PromScript.cpp
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#include "PromScript.h"
#include "../promnet/PromNode.h"
#include "../promnet/PromNet.h"
#include "../../workbench/PromWorkbench.h"

using namespace libboiboites;
namespace coeos {


PromScript::~PromScript() {
	if(node) {delete node; node = 0;}
}

void PromScript::load(const std::string& filename) {
	this->filename = filename;
	setlocale(LC_NUMERIC, "C");
	std::ifstream f(filename);
	if(!f) {ERROR("File doesn't exist : " << filename); throw "not exist";}
	if(!read(f)) {ERROR("Failed loading " << filename); f.close(); throw "failed";}
	f.close();
}

std::ostream& operator<<(std::ostream& os, PromScript* a) {
	a->dump(os);
	return os;
}


void PromScript::save() {
	DBG("Save script " << name << " to " << node->get_absolute_filename());
	save_as(node->get_absolute_filename());
	POPUP("Saved script " << name << " to " << node->get_absolute_filename());
}


void PromScript::parse_comments_annotations() {
	int key_i=-1, v_i=-1, v_j=-1;
	std::string key,value;
	for(uint i=0; i<comments_groups.size(); i++) {
		if(key_i==-1 && v_i==-1 && comments_groups[i]=='@') {key_i = i+1;}
		else if(key_i != -1 && comments_groups[i]=='=') {v_i=i+1;}
		else if(v_i!=-1 && ((isspace(comments_groups[i]) || i==comments_groups.size()-1 || comments_groups[i]=='\n'))) {
			v_j=i;
			key = str_trim(comments_groups.substr(key_i, v_i-key_i-1));
			value = str_trim(comments_groups.substr(v_i,v_j-v_i));
			annotations.set(key,value);
			key_i = -1; v_i = v_j = -1;
		}
	}

	int curline = 0;
	for(uint i=0; i<comments_groups.size(); i++) {
		if(comments_groups[i]=='\n') curline = i+1;
		else if(comments_groups[i]=='@') {
			while(comments_groups[i]!='\n' && i<comments_groups.size()) i++;
			comments_groups.erase(curline, i-curline+1);
			i = curline-1;
		}
	}
}

}
