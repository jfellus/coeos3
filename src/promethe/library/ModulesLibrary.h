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

using namespace libboiboites;
namespace coeos {


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

	inline bool is_type_algo() { return get_type_no() == 14; }

	inline bool is_type_custom() { return TOINT(get("custom")) == 1; }

	std::string get_group_name() {	return is_type_algo() ? get("name") : PROM_GROUP_TYPES[get_type_no()];	}

	inline int get_type_no() { return TOINT(get("type")); }
	inline void set_type_no(int typeno) {properties.set("type", TOSTRING(typeno));	}
	inline void set_author(const std::string s) {properties.set("author", s); }
	inline void set_stars(int nbstars) {properties.set("stars", TOSTRING(nbstars));}

	inline void set_type_custom() {set_type_no(14); properties.set("custom", "1");}
};





struct group_function_pointers;
extern group_function_pointers NN_Core_function_pointers[];

class ModulesLibrary {
public:
	static std::vector<ModuleDef*> defs;

public:

	static void add_promethe_default_libraries() {
		add_neural_groups(PROM_GROUP_TYPES);
		add(NN_Core_function_pointers);
		add_promethe_shared_info("libParallel_Comm.info");
		add_promethe_shared_info("libIHM.info");
		add_promethe_shared_info("libSigProc.info");
		add_promethe_shared_info("libIO_Robot.info");
		add_promethe_shared_info("libNN_IO.info");
		add_promethe_shared_info("libSensors.info");
		add_custom_cpp(TOSTRING(getenv("HOME") << "/" << "bin_leto_prom/.custom_cpp_modules_libs.txt"));
	}

	static void add_neural_groups(const char** names) {
		for(uint i=0; names[i]!=NULL; i++) {
			if(i==14) continue;
			ModuleDef* m = new ModuleDef(names[i]);
			m->set_type_no(i);
		}
	}

	static void add_custom_cpp(const std::string& filename) {
		if(file_has_ext(filename, ".txt")) {
			std::ifstream f(filename);
			std::string line;
			while (std::getline(f, line)) {add_custom_cpp(str_trim(line));}
			f.close();
			return;
		}
		else if(file_has_ext(filename, ".so")) {
			load_custom_cpp_lib(filename);
		} else ERROR("Unknown custom cpp file type : " << filename);
	}

	static void add(const char* filename) {
		if(file_has_ext(filename, ".so")) add_promethe_shared_lib(filename);
	}

	static void add(group_function_pointers* p);

	static void add_promethe_shared_lib(const char* filename) ;
	static void add_promethe_shared_info(const char* filename) ;


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

protected:
	static void load_custom_cpp_lib(const std::string& filename);
};


}

#endif /* MODULESLIBRARY_H_ */
