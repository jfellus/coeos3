/*
 * PromLink.cpp
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#include "PromLink.h"
#include "PromScript.h"

PromLink::PromLink(PromProject* project) {
	this->project = project;
	src = dst =  NULL;
	mem_time_in = mem_time_out = norm = "";
	nb = "";
	computation_mode = 0;
	secondary = 0;
	type = 0;
	script = NULL;
	dv_x = dv_y = "";
	proba = "1";
}

PromLink::PromLink(PromScript* script, std::istream& f) {
	this->script = script;
	this->project = script->project;
	read(f);
}

PromLink::~PromLink() {

}

void PromLink::read(std::istream& f) {
	std::string _src;
	std::string _dst;

	comments = f_read_comments(f);
	f_try_read(f, "liaison entre %s   et ", _src);
	f_try_read(f, "%s , ", _dst);
	f_try_read(f, "type = %u , ", type);
	f_try_read(f, "nbre = %s , ", nb);
	f_try_read(f, "norme = %s\n", norm);
	f_try_read(f,	"                         temps de memorisation entree= %s \n", mem_time_in);
	f_try_read(f, "                         temps de memorisation sortie= %s \n", mem_time_out);
	f_try_read(f, "                         mode de calcul        = %u \n", computation_mode);
	f_try_read(f, "                         secondaire            = %u \n", secondary);



	if (type == No_l_algorithmique || type == No_l_neuro_mod || type == No_l_1_1_non_modif_bloqueur)
		f_try_read(f, "                         nom = %s\n", name);

	if (type == No_l_1_v || type == No_l_1_v_non_modif)
	{
		f_try_read(f, "                         dv_x = %s \n", dv_x);
		f_try_read(f, "                         dv_y = %s \n", dv_y);
	}

	try{f_try_read(f, 	"                         proba = %s \n", proba);}catch(...) { proba = "1";}


	if(_src=="-1" || _dst=="-1") return;
	src = script->get_group_by_no_name(_src); if(!src) throw Exception_GroupNotFound(_src);
	dst = script->get_group_by_no_name(_dst); if(!dst) throw Exception_GroupNotFound(_dst);
}

void PromLink::write(std::ostream& f) {
	f_write_comments(f, comments);
	f << "liaison entre "<< src->no_name << "   et  "<< dst->no_name << " , type = "<< type << " , nbre = "<< nb
			<< " , norme = " << norm << "\n";
	f << "                         temps de memorisation entree= " << mem_time_in << " \n";
	f << "                         temps de memorisation sortie= " << mem_time_out <<" \n";
	f << "                         mode de calcul        = " << computation_mode << " \n";
	f << "                         secondaire            = " << secondary << " \n";



	if (type == No_l_algorithmique || type == No_l_neuro_mod || type == No_l_1_1_non_modif_bloqueur)
		f << "                         nom = " << name << "\n";

	if (type == No_l_1_v || type == No_l_1_v_non_modif) {
		f << "                         dv_x = " << dv_x << " \n";
		f << "                         dv_y = " << dv_y << " \n";
	}

	if(!proba.empty()) f << "                         proba = " << proba << " \n";

}


std::ostream& operator<<(std::ostream& os, PromLink* a) {
	a->dump(os);
	return os;
}
