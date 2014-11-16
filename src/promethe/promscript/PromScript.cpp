/*
 * PromScript.cpp
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#include "PromScript.h"
#include "../promnet/PromNode.h"
#include "../promnet/PromNet.h"


PromScript::~PromScript() {
	if(node) {delete node; node = 0;}
}

void PromScript::load(const std::string& _filename) {
	std::string filename = _filename;
	if(node && node->net) {
	filename = node->net->get_dir() + "/" + path_deploy + "/" + (path_symb.empty() ? path_script : path_symb);
	}
	else this->path_script = _filename;
	setlocale(LC_NUMERIC, "C");
	std::ifstream f(filename);
	if(!f) {ERROR("File doesn't exist : " << filename); throw "not exist";}
	if(!read(f)) {ERROR("Failed loading " << filename); f.close(); throw "failed";}
	f.close();
}

void PromScript::init() {
	path_script = file_change_ext(name, ".script");
	path_symb = file_change_ext(name, ".symb");
	path_draw = file_change_ext(name, ".draw");
	path_res = file_change_ext(name, ".res");
	path_config = (!node || !node->net || node->net->path_config.empty()) ? file_change_ext(name, ".script") : node->net->path_config;
	path_gcd = file_change_ext(name, ".gcd");
	path_dev = file_change_ext(name, ".dev");
	std::string path_deploy = name;
}

std::ostream& operator<<(std::ostream& os, PromScript* a) {
	a->dump(os);
	return os;
}


void PromScript::save() {
	std::string fn = path_symb;
	if(fn.empty()) fn = path_script;
	if(fn.empty()) fn = path_symb = file_change_ext(name, ".symb");
	if(node && node->net) fn = node->net->get_dir() + "/" + path_deploy + "/" + fn;
	save_as(fn);
}
