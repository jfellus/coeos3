/*
 * Compiler.cpp
 *
 *  Created on: 21 déc. 2014
 *      Author: jfellus
 */

#include "Compiler.h"
#include "../../workbench/PromWorkbench.h"

namespace coeos {

Compiler::Compiler() {
	// TODO Auto-generated constructor stub

}

Compiler::~Compiler() {
	// TODO Auto-generated destructor stub
}


void Compiler::create_themis_makefile(PromNode* node) {
	std::ofstream f(get_makefile(node));
	f << "path_symb=" << node->get_absolute_path(node->path_symb) << "\n";
	f << "path_script=" << node->get_absolute_path(node->path_script) << "\n";
	f << "path_config=" << node->get_absolute_path(node->path_config) << "\n";
	f << "path_res=" << node->get_absolute_path(node->path_res) << "\n";
	f << "path_dev=" << node->get_absolute_path(node->path_dev) << "\n";
	f << "path_gcd=" << node->get_absolute_path(node->path_gcd) << "\n";
	f << "path_prt=" << node->get_absolute_path(node->path_prt) << "\n\n";

	f << "logical_name=" << node->script->name << "\n";
	f << "themis_ip=" << "127.0.0.1" << "\n";
	f << "themis_id=" << "1" << "\n\n";

	f << "login=" << "" << "\n";
	f << "computer=" << "" << "\n";
	f << "synchronize_files=" << "" << "\n";
	f << "synchronize_directories=" << "" << "\n\n";

	f << "promethe_args=" << "-S0" << "\n\n";

	f << "all: run" << "\n\n";
	f << "include ~/bin_leto_prom/themis.mk" << "\n\n";

	f << "stop:" << "\n";
	f << "\t" << "-killall -q promethe" << "\n";
	f << "\t" << "-killall -q bpromethe" << "\n";
	f.close();
}


std::string Compiler::get_makefile(PromNode* node) {
	return file_change_ext(node->get_absolute_path(node->path_script), ".mk");
}


void Compiler::compile(PromNode* node) {
	if(node->path_symb.empty() && node->path_script.empty()) {
		ERROR("Script doesn't define any .script or .symb file !");	return;
	}

	if(node->path_script.empty()) node->path_script = file_change_ext(node->path_symb, ".script");
	if(node->path_res.empty()) node->path_res = file_change_ext(node->path_script, ".res");
	if(node->path_config.empty()) node->path_config = file_change_ext(file_absolute_path(node->net->filename), ".config");

	compile_config(node);

	Compiler::create_themis_makefile(node);
	call_make(node, node->get_absolute_path(node->path_res));

	POPUP("Compiled " << node->script->name);
}


void Compiler::compile_config(PromNode* node) {

	// TODO REALLY WRITE CONFIG ! (perhaps from annotations in the .symb)
	if(!file_exists(node->get_absolute_path(node->path_config))) {
		DBG("Compile " << node->get_absolute_path(node->path_config));
		shell("cp ~/bin_leto_prom/default.config '" + node->get_absolute_path(node->path_config) + "'");
	}
}


void Compiler::call_make(PromNode* node, const std::string& target) {
	shell(TOSTRING("make -f '" << get_makefile(node) << "' '" << target << "'"));
}

}
