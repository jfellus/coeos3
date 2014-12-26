/*
 * Launcher.cpp
 *
 *  Created on: 17 nov. 2014
 *      Author: jfellus
 */

#include "Launcher.h"


using namespace libboiboites;
namespace coeos {

class RuntimeListener : public PromNodeThread::IRuntimeListener {
public:
	RuntimeListener() {}
	virtual ~RuntimeListener() {}
	virtual void on_start(PromNodeThread* t) {
		PromWorkbench::cur()->on_start(t);
	}
	virtual void on_stop(PromNodeThread* t) {
		PromWorkbench::cur()->on_stop(t);
	}
};



void Launcher::start(PromProject* project, bool bGui) {
	project->save_net();
	if(project->net->filename.empty()) return;
	for(uint i=0; i<project->net->nodes.size(); i++) {
		start(project->net->nodes[i], bGui);
	}
}

void Launcher::stop(PromProject* project) {
	for(uint i=0; i<project->net->nodes.size(); i++) {
		stop(project->net->nodes[i]);
	}
}

void Launcher::start(PromNode* node, bool bGui) {
	Compiler::compile(node);
	create_thread(node, bGui);
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



std::vector<PromNodeThread*> Launcher::threads;

static void* _start_thread(void* p) {
	PromNodeThread* node = (PromNodeThread*)p;
	node->node->properties.set("started", "yes");
	node->run();
	node->node->properties.set("started", "no");
	delete node;
	return 0;
}



void Launcher::create_thread(PromNode* node, bool bGui) {
	PromNodeThread* t = new PromNodeThread(node, bGui);
	threads.push_back(t);
	t->add_runtime_listener(new RuntimeListener());
}


PromNodeThread::PromNodeThread(PromNode* node, bool bGui) : node(node), bGui(bGui) {
	Launcher::threads.push_back(this);
	pthread_create(&thread, NULL, _start_thread, this);
}

PromNodeThread::~PromNodeThread() {
	vector_remove(Launcher::threads, this);
}

void PromNodeThread::run() {
	bStarted = true;
	for(uint i=0; i<runtime_listeners.size(); i++) runtime_listeners[i]->on_start(this);
	//shell(Launcher::get_start_file(node));
	Compiler::call_make(node, bGui ? "run_gui" : "run_blind");
	bStarted = false;
	for(uint i=0; i<runtime_listeners.size(); i++) runtime_listeners[i]->on_stop(this);
}


}
