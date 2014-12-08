/*
 * GroupPromScript.h
 *
 *  Created on: 8 nov. 2014
 *      Author: jfellus
 */

#ifndef GROUPPROMSCRIPT_H_
#define GROUPPROMSCRIPT_H_


#include <module/Group.h>
#include "../promethe/promscript/PromScript.h"



using namespace libboiboites;
namespace coeos {




class GroupPromScript : public Group {
public:
	PromScript* script = NULL;
	PromProject* project = NULL;
	PromNode* node = NULL;
public:
	GroupPromScript(PromScript* script);
	virtual ~GroupPromScript();

	virtual void realize();


	virtual void detach(bool bSlave = false);
	virtual void attach();

};


}

#endif /* GROUPPROMSCRIPT_H_ */
