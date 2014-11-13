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

void ModulesLibrary::read_promethe_shared_lib(const char* _filename) {
	std::string filename = _filename;
	if(!file_is_absolute(filename)) filename = TOSTRING(getenv("HOME") << "/bin_leto_prom/Libraries/prom_user/" << filename);

#ifdef Darwin
	filename = file_change_ext(filename.c_str(), ".dylib");
#else
	filename = file_change_ext(filename.c_str(), ".so");
#endif

	/* Load dynamically loaded library */
	void* handle = dlopen(filename.c_str(), RTLD_LAZY);
	if(handle==NULL) {ERROR("Can't open promethe shared library " << filename << " (" << dlerror() << ")"); return;}

	type_group_function_pointers *group_function_pointers;
	group_function_pointers = (type_group_function_pointers*)dlsym(handle, "group_function_pointers");
	if(!group_function_pointers) {ERROR("Can't find group_function_pointers in " << filename); return; }

	add(group_function_pointers);

	dlclose(handle);
}

void ModulesLibrary::add(type_group_function_pointers* p) {
	for(uint i=0; p[i].name != NULL; i++) {
		new ModuleDef(p[i].name);
	}
}





ModuleDef::ModuleDef(const std::string& name) {
	if(!ModulesLibrary::get(name))	ModulesLibrary::defs.push_back(this);
	else ERROR("Module already defined : " << name);

	properties.set_from_string("name", name);
	std::string svg = SVGDefinitions::get(name);
	if(svg.empty()) svg = SVGDefinitions::get("module_algo"); // TODO
	properties.set_from_string("svg", svg);

	properties.set_from_string("type", "Algorithmic module"); // TODO
	properties.set_from_string("author", "Jerome le bo gosse"); // TODO
	properties.set_from_string("stars", TOSTRING((rand()%5)));

}

ModuleDef::~ModuleDef() {
	vector_remove(ModulesLibrary::defs,this);
}
