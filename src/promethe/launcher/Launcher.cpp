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
	Compiler::compile(node);
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



void Launcher::create_launcher_program(PromNode* node) {
	std::string makefile = Compiler::get_makefile(node);
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
