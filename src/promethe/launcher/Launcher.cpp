/*
 * Launcher.cpp
 *
 *  Created on: 17 nov. 2014
 *      Author: jfellus
 */

#include "Launcher.h"


using namespace libboiboites;
namespace coeos {

class TestListener : public PromNodeThread::IRuntimeListener {
public:
	TestListener() {}
	virtual ~TestListener() {}
	virtual void on_start(PromNodeThread* t) {
		STATUS("STARTED : " << t->node->script->name);
	}
	virtual void on_stop(PromNodeThread* t) {
		STATUS("STOPPED : " << t->node->script->name);
		ERROR("STOPPED : " << t->node->script->name);
	}
};



void Launcher::start(PromProject* project) {
	project->save_net();
	if(project->net->filename.empty()) return;
	for(uint i=0; i<project->net->nodes.size(); i++) {
		start(project->net->nodes[i]);
	}
}

void Launcher::stop(PromProject* project) {
	for(uint i=0; i<project->net->nodes.size(); i++) {
		stop(project->net->nodes[i]);
	}
}

void Launcher::start(PromNode* node) {
	if(node->path_script.empty()) {
		if(node->path_symb.empty()) {
			ERROR("Script doesn't define any .script or .symb file !");
			return;
		}

		// TODO SHOULD BUILD SYMB->SCRIPT
		DBG("WARNING : symb->script not implemented yet (see Nils)");
		if(node->path_script.empty()) node->path_script = file_change_ext(node->path_symb, ".script");
		shell("cp \"" + node->get_absolute_path(node->path_symb) + "\" \"" + node->get_absolute_path(node->path_script) + "\"");
	}
	if(node->path_res.empty()) node->path_res = file_change_ext(node->path_script, ".res");


	// TODO WRITE CONFIG !
	if(node->path_config.empty()) node->path_config = file_change_ext(file_absolute_path(node->net->filename), ".config");
	shell("cp ~/bin_leto_prom/default.config \"" + node->get_absolute_path(node->path_config) + "\"");

	create_themis_makefile(node);
	create_launcher_program(node);
	create_thread(node);
}

std::string Launcher::get_stop_file(PromNode* node) {
	return file_dirname(node->get_absolute_path(node->path_script)) + "/stop_" + str_replace(node->script->name, " ", "_");
}

std::string Launcher::get_start_file(PromNode* node) {
	return file_dirname(node->get_absolute_path(node->path_script)) + "/" + str_replace(node->script->name, " ", "_");
}


void Launcher::stop(PromNode* node) {
	//shell(Launcher::get_stop_file(node));
	shell("killall -q bpromethe");
	shell("killall -q promethe");
}


void Launcher::create_themis_makefile(PromNode* node) {
	std::string path_script = node->get_absolute_path(node->path_script);
	std::string path_makefile = get_makefile(node);
	std::ofstream f(path_makefile);
	f << "path_script=" << path_script << "\n";
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

	f << "promethe_binary=" << "~/bin_leto_prom/bpromethe" << "\n";
	f << "promethe_args=" << "-S0" << "\n\n";

	f << "all: run" << "\n\n";
	f << "include ~/bin_leto_prom/themis.mk" << "\n\n";

	f << "stop:" << "\n";
	f << "\t" << "-killall -q promethe" << "\n";
	f << "\t" << "-killall -q bpromethe" << "\n";
	f.close();
}


void Launcher::create_launcher_program(PromNode* node) {
	std::string makefile = get_makefile(node);
	std::string launch_file = get_start_file(node);
	std::string stop_file = get_stop_file(node);

	std::ofstream fl(launch_file);
	fl << "#!/bin/bash" << "\n\n";
	fl << "make -f \"" << makefile << "\"\n";
	fl.close();
	chmod(launch_file.c_str(), S_IRUSR|S_IXUSR);

	std::ofstream fs(stop_file);
	fl << "#!/bin/bash" << "\n\n";
	fl << "make -f \"" << makefile << "\" stop > /dev/null 2>/dev/null" << "\n";
	fl.close();
	chmod(stop_file.c_str(), S_IRUSR|S_IXUSR);
}

std::string Launcher::get_makefile(PromNode* node) {
	return file_change_ext(node->get_absolute_path(node->path_script), ".mk");
}


std::vector<PromNodeThread*> Launcher::threads;

static void* _start_thread(void* p) {
	PromNodeThread* node = (PromNodeThread*)p;
	node->run();
	//delete node;
	return 0;
}



void Launcher::create_thread(PromNode* node) {
	PromNodeThread* t = new PromNodeThread(node);
	threads.push_back(t);
	t->add_runtime_listener(new TestListener());
}


PromNodeThread::PromNodeThread(PromNode* node) : node(node) {
	Launcher::threads.push_back(this);
	pthread_create(&thread, NULL, _start_thread, this);
}

PromNodeThread::~PromNodeThread() {
	vector_remove(Launcher::threads, this);
}

void PromNodeThread::run() {
	bStarted = true;
	for(uint i=0; i<runtime_listeners.size(); i++) runtime_listeners[i]->on_start(this);
	shell(Launcher::get_start_file(node));
	bStarted = false;
	for(uint i=0; i<runtime_listeners.size(); i++) runtime_listeners[i]->on_stop(this);
}


}
