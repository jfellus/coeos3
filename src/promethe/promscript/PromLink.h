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
	std::string norm = "0.1";

	std::string dv_x, dv_y;

	std::string nb = "1";

	std::string mem_time_in = "0.0", mem_time_out = "0.0";
	int computation_mode = 0;
	int secondary = 0;

	std::string proba;

	PromScript* script;

	std::string comments;
public:
	PromLink(PromProject* project, int type);
	PromLink(PromScript* script, std::istream& f);
	PromLink(const PromLink& l);

	virtual ~PromLink();

	bool is_type_algo() {return type==5; }

	PromLink* copy();

	void read(std::istream& f);
	void write(std::ostream& f);

	virtual void dump(std::ostream& os) {
		os << "PromLink(" << src << ", " << dst << "," << type << ")";
	}
	friend std::ostream& operator<<(std::ostream& os, PromLink* a);
};

std::ostream& operator<<(std::ostream& os, PromLink* a);


#endif /* PROMLINK_H_ */
