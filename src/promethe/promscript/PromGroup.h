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
#include "../library/ModulesLibrary.h"


using namespace libboiboites;
namespace coeos {




class PromScript;
class PromProject;

class PromGroup {
public:
	PromProject* project = NULL;

	Properties annotations;

	std::string nb_neurons = "1";
	std::string width = "1", height = "1";

	uint type;
	std::string group;
	std::string custom_function = "";
	uint type2 = 0;

	int reverse = 0, debug = 0;

	int posx = 0, posy = 0, p_posx = 0, p_posy = 0;

	std::string learning_rate = "0.1";
	std::string simulation_speed = "1.0";
	std::string threshold = "0.0";
	std::string alpha = "0.0";
	std::string tolerance = "0.0";
	std::string dvp = "0", dvn = "0";
	std::string nbre_de_1 = "0";
	std::string sigma_f = "0.0";
	std::string noise_level = "0.0";
	std::string time_spectrum_min = "0.0",time_spectrum_max = "0.0";


	std::string no_name; // TODO : Should disappear !
	int time_scale = 0; // (inferred)



	PromScript* script;

	std::string comments;
public:
	PromGroup(PromScript* script, const std::string& nametype);
	PromGroup(PromScript* script, std::istream& f);
	PromGroup(const PromGroup& g);

	virtual ~PromGroup();


	std::string get_type() {
		return custom_function.empty() ?  (is_type_algo() ? group : PROM_GROUP_TYPES[type]) : custom_function;
	}

	std::string get_name() {
		return is_type_algo() ? annotations.get_as_string("name") : group;
	}

	void set_type(const std::string& s) {
		custom_function = "";
		ModuleDef* d = ModulesLibrary::get(s);
		if(d) {
			type = d->get_type_no();
			if(d->is_type_custom()) {custom_function = s; group = "f_custom_c++";}
			else if(d->is_type_algo()) {group = s;}
		} else {
			type = 14; group = s;
		}
	}

	void set_name(const std::string& s) {
		if(!is_type_algo()) group = s;
		else annotations.set("name", s);
	}

	bool is_type_algo() { return type==14; }


	PromGroup* copy();

	void read(std::istream& f);

	void write(std::ostream& f);

	virtual void dump(std::ostream& os) {
		os << "PromGroup(" << no_name << ", " << get_type() << "," << get_name() << ")";
	}
	friend std::ostream& operator<<(std::ostream& os, PromGroup* a);

protected:
	void parse_comments_annotations();
};

std::ostream& operator<<(std::ostream& os, PromGroup* a);



}

#endif /* PROMGROUP_H_ */
