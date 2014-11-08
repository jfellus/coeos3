/*
 * PromGroup.h
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMGROUP_H_
#define PROMGROUP_H_

#include <util/utils.h>
#include "../promtools.h"
#include <module/Property.h>


class PromScript;
class PromProject;

class PromGroup {
public:
	PromProject* project = NULL;
	std::string no_name;
	std::string name;
	uint type;
	std::string nb_neurons;
	std::string threshold;
	std::string width, height;

	std::string learning_rate;
	std::string simulation_speed;
	uint type2;

	std::string group;
	std::string custom_function = "";
	Properties annotations;

	int reverse, debug;
	int time_scale;

	int posx, posy, p_posx, p_posy;

	std::string alpha;

	std::string tolerance;
	std::string dvp, dvn;
	std::string nbre_de_1;
	std::string sigma_f;
	std::string noise_level;
	std::string time_spectrum_min,time_spectrum_max;

	PromScript* script;

	std::string comments;
public:
	PromGroup(PromProject* project);
	PromGroup(PromScript* script, std::istream& f);

	virtual ~PromGroup();


	std::string get_text() {
		return custom_function.empty() ?  (is_type_algo() ? group : get_type_as_string()) : custom_function;
	}

	bool is_type_algo() { return type==14; }

	std::string get_type_as_string() {
		return PROM_GROUP_TYPES[type];
	}


	void read(std::istream& f);

	void write(std::ostream& f);

	virtual void dump(std::ostream& os) {
		os << "PromGroup(" << no_name << ", " << group << "," << type << ")";
	}
	friend std::ostream& operator<<(std::ostream& os, PromGroup* a);

protected:
	void parse_comments_annotations();
};

std::ostream& operator<<(std::ostream& os, PromGroup* a);



#endif /* PROMGROUP_H_ */
