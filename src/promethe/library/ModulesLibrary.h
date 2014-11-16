/*
 * ModulesLibrary.h
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#ifndef MODULESLIBRARY_H_
#define MODULESLIBRARY_H_

#include <module/Property.h>
#include "../promtools.h"



class ModuleDef {
public:
	Properties properties;
public:
	ModuleDef(const std::string& name);
	virtual ~ModuleDef();

	virtual std::string to_JSON() {
		return properties.to_JSON();
	}

	std::string get(const std::string property) {
		return properties.get(property) ? properties.get(property)->get_value_as_string() : "";
	}

	inline bool is_type_algo() {
		return true; // TODO
	}

	std::string get_group_name() {
		return is_type_algo() ? get("name") : PROM_GROUP_TYPES[get_type_no()];
	}

	inline int get_type_no() { return TOINT(get("type")); }


};





struct group_function_pointers;
extern group_function_pointers NN_Core_function_pointers[];

class ModulesLibrary {
public:
	static std::vector<ModuleDef*> defs;

public:

	static void add_promethe_default_libraries() {
		new ModuleDef("f_prout");
		new ModuleDef("f_affiche_image_from_extension");
		add(NN_Core_function_pointers);
//		add("libSigProc_blind_release.so");
//		add("libIHM_blind_release.so");
//		add("libIO_Robot_blind_release.so");			TODO !!!!
//		add("libParallel_Comm_blind_release.so");
//		add("libNN_IO_blind_release.so");
//		add("libSensors_blind_release.so");
	}


	static void add(const char* filename) {
		if(file_has_ext(filename, ".so")) read_promethe_shared_lib(filename);
	}

	static void add(group_function_pointers* p);

	static void read_promethe_shared_lib(const char* filename) ;


	static ModuleDef* get(const std::string& name) {
		for(uint i=0; i<defs.size(); i++) {
			if(defs[i]->get("name")==name) return defs[i];
		}
		return NULL;
	}

	static std::string to_JSON() {
		std::string s = "[ ";
		for(uint i=0; i<defs.size(); i++) {
			if(i!=0) s+= ", ";
			s+= defs[i]->to_JSON();
		}
		s += "]";
		return s;
	}
};

#endif /* MODULESLIBRARY_H_ */
