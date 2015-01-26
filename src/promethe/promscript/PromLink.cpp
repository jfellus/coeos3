/*
 * PromLink.cpp
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#include "PromLink.h"
#include "PromScript.h"


using namespace libboiboites;
namespace coeos {



PromLink::PromLink(PromProject* project, int type) {
	this->project = project;
	src = dst =  NULL;
	this->type = type;
	script = NULL;
}

PromLink::PromLink(const PromLink& l) {
	project = l.project;
	src = l.src;
	dst = l.dst;
	type = l.type;
	script = l.script;
	name = l.name;
	norm = l.norm;
	dv_x = l.dv_x;
	dv_y = l.dv_y;
	nb = l.nb;
	mem_time_in = l.mem_time_in;
	mem_time_out = l.mem_time_out;
	computation_mode = l.computation_mode;
	secondary = l.secondary;
	proba = l.proba;
	comments = l.comments;
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
	parse_comments_annotations();

	f_try_read(f, "liaison entre %s", _src);
	f_try_read(f, "et %s ,", _dst);
	f_try_read(f, "type = %u ,", type);
	f_try_read(f, "nbre = %s ,", nb);
	f_try_read(f, "norme = %s", norm);
	f_try_read(f, "temps de memorisation entree= %s", mem_time_in);
	f_try_read(f, "temps de memorisation sortie= %s", mem_time_out);
	f_try_read(f, "mode de calcul = %u", computation_mode);
	f_try_read(f, "secondaire = %u", secondary);



	if (type == No_l_algorithmique || type == No_l_neuro_mod || type == No_l_1_1_non_modif_bloqueur)
		f_try_read(f, "nom = %s", name);

	if (type == No_l_1_v || type == No_l_1_v_non_modif)
	{
		f_try_read(f, "dv_x = %s", dv_x);
		f_try_read(f, "dv_y = %s", dv_y);
	}

	try{f_try_read(f, "proba = %s", proba);}catch(...) { proba = "1";}


	if(_src=="-1" || _dst=="-1") return;

	if(name==".") name="";

	src = script->get_group_by_no_name(_src); if(!src) throw Exception_GroupNotFound(_src);
	dst = script->get_group_by_no_name(_dst); if(!dst) throw Exception_GroupNotFound(_dst);
}

void PromLink::write(std::ostream& f) {
	f_write_comments(f, comments);
	for(uint i=0; i<annotations.size(); i++) {
		if(annotations[i]->get_value_as_string().empty()) continue;
		f << "%@" << annotations[i]->name << "=" << annotations[i]->get_value_as_string() << "\n";
	}

	f << "liaison entre "<< src->no_name << "   et  "<< dst->no_name << " , type = "<< type << " , nbre = "<< nb
			<< " , norme = " << norm << "\n";
	f << "temps de memorisation entree= " << mem_time_in << " \n";
	f << "temps de memorisation sortie= " << mem_time_out <<" \n";
	f << "mode de calcul = " << computation_mode << " \n";
	f << "secondaire = " << secondary << " \n";


	if (type == No_l_algorithmique || type == No_l_neuro_mod || type == No_l_1_1_non_modif_bloqueur)
		f << "nom = " << (name.empty() ? "." : name) << "\n";

	if (type == No_l_1_v || type == No_l_1_v_non_modif) {
		f << "dv_x = " << dv_x << " \n";
		f << "dv_y = " << dv_y << " \n";
	}

	if(!proba.empty()) f << "proba = " << proba << " \n";

}



void PromLink::parse_comments_annotations() {
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


PromLink* PromLink::copy() {
	return new PromLink(*this);
}


std::ostream& operator<<(std::ostream& os, PromLink* a) {
	a->dump(os);
	return os;
}

}
