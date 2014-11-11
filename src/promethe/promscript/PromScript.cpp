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

