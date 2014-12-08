/*
 * PromGroupCreator.cpp
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#include "PromGroupCreator.h"

static std::string _last_nametype = "f_debut";


using namespace libboiboites;
namespace coeos {


PromGroupCreator::PromGroupCreator(PromProject* project) : ModuleCreator(), project(project) {
	nametype = _last_nametype;
}


PromGroupCreator::PromGroupCreator(PromProject* project, const std::string& nametype) : ModuleCreator(), project(project) {
	this->nametype = nametype;
	_last_nametype = nametype;
}


}
