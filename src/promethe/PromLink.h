/*
 * PromLink.h
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMLINK_H_
#define PROMLINK_H_

#include "PromGroup.h"

class PromScript;
class PromProject;

class PromLink {
public:
	PromProject* project = NULL;
	PromGroup *src = NULL, *dst = NULL;

	std::string name;

	int type;
	std::string norm;

	std::string dv_x, dv_y;

	std::string nb;

	std::string mem_time_in, mem_time_out;
	int computation_mode;
	int secondary;

	std::string proba;

	PromScript* script;

	std::string comments;
public:
	PromLink(PromProject* project);
	PromLink(PromScript* script, std::istream& f);

	virtual ~PromLink();

	bool is_type_algo() {return type==5; }

	void read(std::istream& f);
	void write(std::ostream& f);

	virtual void dump(std::ostream& os) {
		os << "PromLink(" << src << ", " << dst << "," << type << ")";
	}
	friend std::ostream& operator<<(std::ostream& os, PromLink* a);
};

std::ostream& operator<<(std::ostream& os, PromLink* a);


#endif /* PROMLINK_H_ */
