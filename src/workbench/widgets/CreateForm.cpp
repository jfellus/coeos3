/*
 * CreateForm.cpp
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#include "CreateForm.h"
#include "../../promethe/library/ModulesLibrary.h"
#include "../../workbench/PromWorkbench.h"


using namespace libboiboites;
namespace coeos {



std::string CreateForm::answer(const std::string& request, const std::string& data) {
	if(!request.empty()) {
		PromWorkbench::cur()->create_module(request);
		return "ok";
	}
	return ModulesLibrary::to_JSON();
}



}
