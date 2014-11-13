/*
 * CreateForm.cpp
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#include "CreateForm.h"
#include "../../promethe/library/ModulesLibrary.h"


std::string CreateForm::answer(const std::string& request, const std::string& data) {
	return ModulesLibrary::to_JSON();
}
