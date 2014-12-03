/*
 * ModulesLibrary.cpp
 *
 *  Created on: 13 nov. 2014
 *      Author: jfellus
 */

#include "modules_library_defs.h"
#include "ModulesLibrary.h"
#include <dlfcn.h>
#include <style/SVGDefinitions.h>

std::vector<ModuleDef*> ModulesLibrary::defs;


void ModulesLibrary::add_promethe_shared_info(const char* _filename) {
	std::string filename = _filename;
	if(!file_is_absolute(filename)) filename = TOSTRING(getenv("HOME") << "/bin_leto_prom/Libraries/prom_user/" << filename);
	std::ifstream f(filename);
	std::string s;
	while(getline(f, s).good()) {
		ModuleDef* m = new ModuleDef(s);
		m->set_type_no(14);
	}
	f.close();
}

void ModulesLibrary::add_promethe_shared_lib(const char* _filename) {
	std::string filename = _filename;
	if(!file_is_absolute(filename)) filename = TOSTRING(getenv("HOME") << "/bin_leto_prom/Libraries/prom_user/" << filename);

#ifdef Darwin
	filename = file_change_ext(filename.c_str(), ".dylib");
#else
	filename = file_change_ext(filename.c_str(), ".so");
#endif

	/* Load dynamically loaded library */
	void* handle = dlopen(filename.c_str(), RTLD_LAZY | RTLD_LOCAL);
	if(handle==NULL) {ERROR("Can't open promethe shared library " << filename << " (" << dlerror() << ")"); return;}

	type_group_function_pointers *group_function_pointers;
	group_function_pointers = (type_group_function_pointers*)dlsym(handle, "group_function_pointers");
	if(!group_function_pointers) {ERROR("Can't find group_function_pointers in " << filename); return; }

	add(group_function_pointers);

	dlclose(handle);
}

void ModulesLibrary::add(type_group_function_pointers* p) {
	for(uint i=0; p[i].name != NULL; i++) {
		ModuleDef* m = new ModuleDef(p[i].name);
		m->set_type_no(14);
	}
}





ModuleDef::ModuleDef(const std::string& name) {
	if(!ModulesLibrary::get(name))	ModulesLibrary::defs.push_back(this);
	else {
		//ERROR("Module already defined : " << name); // TODO Should raise a warning
	}

	properties.set("name", name);
	std::string svg = SVGDefinitions::get(name);
	if(svg.empty()) svg = SVGDefinitions::get("module_algo"); // TODO
	properties.set("svg", svg);

	set_type_no(14);
	set_author("anonymous");
	set_stars(rand()%5);
}

ModuleDef::~ModuleDef() {
	vector_remove(ModulesLibrary::defs,this);
}


void ModulesLibrary::load_custom_cpp_lib(const std::string& filename) {
	shell(TOSTRING("nm \"" << filename  << "\" | grep '__custom_create' > /tmp/.coeos_load_custom_cpp_lib"));
	std::ifstream f("/tmp/.coeos_load_custom_cpp_lib");
	if(!f) {ERROR("Unable to load custom cpp lib " << filename); return;}
	std::string line;
	while (std::getline(f, line)) {
		std::string fname = line.substr(line.find("__custom_create_")+strlen("__custom_create_"));
		ModuleDef* m = new ModuleDef(fname);
		m->set_type_custom();
	}
	f.close();
	unlink("/tmp/.coeos_load_custom_cpp_lib");
	return;
}
