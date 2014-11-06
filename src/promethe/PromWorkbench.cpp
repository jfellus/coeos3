/*
 * PromWorkbench.cpp
 *
 *  Created on: 22 oct. 2014
 *      Author: jfellus
 */

#include "PromWorkbench.h"
#include "promethe_modules.h"
#include "PromProject.h"

PromWorkbench* PromWorkbench::cur() { return dynamic_cast<PromWorkbench*>(Workbench::cur()); }

void PromWorkbench::open(const std::string& filename) {
	if(project) close();
	project = new PromProject();
	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb")) {
		try {
			PromScript* s = new PromScript(filename);
			project->net->add(new PromNode(project->net, s));
			project->add(s);
		} catch(...) {}
	} else if(file_has_ext(filename, ".net")) {
		PromNet* net = new PromNet(filename);
		project->load_net(net);
		project->layout_scripts();
	} else {
		ERROR("Unknown file format " << filename);
	}
	document->update_links_layers();
	canvas->update_layers();
	canvas->zoom_all();
}

void PromWorkbench::save(const std::string& filename) {
	if(!project) return;
	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb"))
		project->save_to_single_script(filename);
	else if(file_has_ext(filename, ".net"))
		project->save_net(filename);
	else
		ERROR("Can't save - unknown extension : " << filename);
}

void PromWorkbench::new_document() {
	if(project) close();
	project = new PromProject();
	canvas->zoom_reset();
}

void PromWorkbench::close() {
	if(!project) return;
	delete project;
	project = 0;
}

void PromWorkbench::create_module() {
	canvas->start_creator(new PromGroupCreator(project));
}

void PromWorkbench::create_link() {
	canvas->start_creator(new PromLinkCreator(project));
}
