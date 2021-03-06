/*
 * PromGroup.cpp
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#include "PromGroup.h"
#include "PromScript.h"
#include "../library/ModulesLibrary.h"
#include <util/utils.h>


using namespace libboiboites;
namespace coeos {


static int _next_free_no_name = 1;


PromGroup::PromGroup(PromScript* script, const std::string& nametype) {
	this->script = script;
	this->project = script->project;

	ModuleDef* md = ModulesLibrary::get(nametype);
	if(!md) throw TOSTRING("No such module definition : " << nametype);

	type = md->get_type_no();
	if(md->is_type_custom()) {
		custom_function = md->get_group_name();
		group = "f_custom_c++";
	}
	else if(md->is_type_algo()) group = md->get_group_name();
	else group = "";

	nb_neurons = "1";
	width = height = "1";
	no_name = script->new_noname();

	annotations.set("custom_function", &custom_function);
}

PromGroup::PromGroup(PromScript* script, std::istream& f) {
	this->script = script;
	this->project = script->project;

	annotations.set("custom_function", &custom_function);

	read(f);
}

PromGroup::PromGroup(const PromGroup& g) {
	project = g.script->project;
	script = g.script;

	type = g.type;
	group = g.group;
	nb_neurons = g.nb_neurons;
	width = g.width;
	height = g.height;
	no_name = script->new_noname();

	posx = g.posx;
	posy = g.posy;

	nb_neurons = g.nb_neurons;
	width = g.width; height = g.height;

	custom_function = g.custom_function;
	type2 = g.type2;
	debug = g.debug;
	reverse = g.reverse;

	p_posx = g.p_posx; p_posy = g.p_posy;

	learning_rate = g.learning_rate;
	simulation_speed = g.simulation_speed;
	threshold = g.threshold;
	alpha = g.alpha;
	tolerance = g.tolerance;
	dvp = g.dvp, dvn = g.dvn;
	nbre_de_1 = g.nbre_de_1;
	sigma_f = g.sigma_f;
	noise_level = g.noise_level;
	time_spectrum_min = g.time_spectrum_min;
	time_spectrum_max = g.time_spectrum_max;

	time_scale = g.time_scale;
	comments = g.comments;

	annotations.set("custom_function", &custom_function);

	script->add_group(this);
}

PromGroup::~PromGroup() {
	// TODO Auto-generated destructor stub
}

void PromGroup::parse_comments_annotations() {
	int key_i=-1, v_i=-1, v_j=-1;
	std::string key,value;
	for(uint i=0; i<comments.size(); i++) {
		if(key_i==-1 && v_i==-1 && comments[i]=='@') {key_i = i+1;}
		else if(key_i != -1 && comments[i]=='=') {v_i=i+1;}
		else if(v_i!=-1 && ((isspace(comments[i]) || i==comments.size()-1 || comments[i]=='\n'))) {
			v_j=i;
			key = str_trim(comments.substr(key_i, v_i-key_i-1));
			value = str_trim(comments.substr(v_i,v_j-v_i));
			annotations.set(key,value);
			key_i = -1; v_i = v_j = -1;
		}
	}

	int curline = 0;
	for(uint i=0; i<comments.size(); i++) {
		if(comments[i]=='\n') curline = i+1;
		else if(comments[i]=='@') {
			while(comments[i]!='\n' && i<comments.size()) i++;
			comments.erase(curline, i-curline+1);
			i = curline-1;
		}
	}
}

void PromGroup::read(std::istream& f) {
	comments = f_read_comments(f);
	parse_comments_annotations();

	f_try_read(f, "groupe = %s ,", no_name);
	_next_free_no_name = MAX(_next_free_no_name, TOINT(no_name)+1);

	f_try_read(f, "type = %u ,", type);
	f_try_read(f, "nbre neurones = %s ,", nb_neurons);
	f_try_read(f, "seuil = %s", threshold);
	f_try_read(f, "taillex = %s ,", width);
	f_try_read(f, "tailley = %s", height);
	f_try_read(f, "learning rate = %s", learning_rate);


	if(type==0 || type==23 || type==42) f_try_read(f, "alpha = %s", alpha);
	if (type == No_Winner || type == No_Winner_Macro || type == No_Sutton_Barto || type == No_PCR)
		try {f_try_read(f, "noise_level = %s", noise_level);} catch(...) {}

	f_try_read(f, "simulation speed = %s", simulation_speed);

	if (type == No_Winner_Colonne || type == No_PTM || type == No_PLG || type == No_Winner_Selectif)
		f_try_read(f, "tolerance = %s", tolerance);
	  if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro) {
		  f_try_read(f, "dvp = %s ,", dvp);
		  f_try_read(f, "dvn = %s", dvn);
	  }
	  if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro)
		  f_try_read(f, "alpha = %s", alpha);
	  if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro || type == No_SAW)
		  f_try_read(f, "nbre_de_1 = %s", nbre_de_1);
	  if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro)
		  f_try_read(f, "sigma_f = %s", sigma_f);
	  if (type == No_Kohonen) {
		  f_try_read(f, "dvp = %s ,", dvp);
		  f_try_read(f, "dvn = %s", dvn);
	  }
	  if (type == No_Granular) {
		  f_try_read(f, "time_spectrum min = %f", time_spectrum_min);
		  f_try_read(f, "time_spectrum max = %f", time_spectrum_max);
	  }

	f_try_read(f, "type2  = %u", type2);
	try {f_try_read(f, "groupe = %s", group);} catch(...) {}
	f_try_read(f, "posx = %s ,", posx);
	f_try_read(f, "posy = %s", posy);
	f_try_read(f, "reverse = %u", reverse);
	f_try_read(f, "p_posx = %s ,", p_posx);
	f_try_read(f, "p_posy = %s", p_posy);
	f_try_read(f, "debug = %u", debug);
	f_try_read(f, "ech_temps = %u", time_scale);

	if(is_type_algo() && annotations.get("name")) set_name(annotations.get_as_string("name"));
}

void PromGroup::write(std::ostream& f) {
	// Preparation
	if(no_name.empty()) ERROR("EMPTY NO_NAME !");

	// Writing
	f_write_comments(f, comments);
	for(uint i=0; i<annotations.size(); i++) {
		if(annotations[i]->get_value_as_string().empty()) continue;
		f << "%@" << annotations[i]->name << "=" << annotations[i]->get_value_as_string() << "\n";
	}


	f << "groupe = " << no_name << " , type = "<< type << " , nbre neurones = "
			<< nb_neurons << " , seuil = "<< threshold << "\n";
	f << "taillex = "<< width << " , tailley = " << height << "\n";
	f << "learning rate = " << learning_rate << " \n";

	if(type==0 || type==23 || type==42) f << "alpha = "<< alpha << " \n";
	if (type == No_Winner || type == No_Winner_Macro || type == No_Sutton_Barto || type == No_PCR)
		f << "noise_level = " << noise_level << " \n";

	f << "simulation speed = " << simulation_speed << " \n";

	if (type == No_Winner_Colonne || type == No_PTM || type == No_PLG || type == No_Winner_Selectif)
		f << "tolerance = " << tolerance << "\n";
	if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro) {
		f << "dvp       = "<< dvp <<" , dvn = "<< dvn << "\n";
	}
	if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro)
		f << "alpha     = "<< alpha << " \n";
	if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro || type == No_SAW)
		f << "nbre_de_1 = "<< nbre_de_1 << " \n";
	if (type == No_PTM || type == No_PLG || type == No_Winner_Selectif || type == No_Macro_Colonne || type == No_Winner || type == No_Winner_Macro)
		f << "sigma_f   = " << sigma_f << " \n";
	if (type == No_Kohonen) {
		f << "dvp       = "<< dvp <<" , dvn = "<< dvn << "\n";
	}
	if (type == No_Granular) {
		f << "time_spectrum min = " << time_spectrum_min << " \n";
		f << "time_spectrum max = " << time_spectrum_max << " \n";
	}

	f << "type2  = "<< type2 << " \n";
	f << "groupe = " << group << "\n";
	f << "posx = " << posx << " , posy = " << posy << "\n";
	f << "reverse = " << reverse << "\n";
	f << "p_posx = " << p_posx << " , p_posy = " << p_posy << "\n";
	f << "debug = " << debug << "\n";
	f << "ech_temps = " << time_scale << "\n";

	f << "\n";
}


PromGroup* PromGroup::copy() {
	return new PromGroup(*this);
}


std::ostream& operator<<(std::ostream& os, PromGroup* a) {
	a->dump(os);
	return os;
}


}
