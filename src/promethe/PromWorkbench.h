/*
 * PromWorkbench.h
 *
 *  Created on: 22 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMWORKBENCH_H_
#define PROMWORKBENCH_H_

#include "../workbench/Workbench.h"

class PromProject;

class PromWorkbench : public Workbench {
public:
	PromProject* project = NULL;
public:
	PromWorkbench() {}
	virtual ~PromWorkbench() {}

	virtual void new_document();
	virtual void close();
	virtual void open(const std::string& filename);
	virtual void save(const std::string& filename);
	virtual void create_module();
	virtual void create_link();

	static PromWorkbench* cur();
};

#endif /* PROMWORKBENCH_H_ */
