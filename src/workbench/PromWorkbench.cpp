/*
 * PromWorkbench.cpp
 *
 *  Created on: 22 oct. 2014
 *      Author: jfellus
 */

#include "PromWorkbench.h"
#include "../promethe/PromProject.h"
#include "../creators/PromGroupCreator.h"
#include "../creators/PromLinkCreator.h"
#include "../creators/PromScriptCreator.h"
#include "../promethe/library/ModulesLibrary.h"



static void on_create_script() { PromWorkbench::cur()->create_script(); }
static void on_import_script() { PromWorkbench::cur()->import(); }
static void on_export_script() { PromWorkbench::cur()->export_script(); }

PromWorkbench* PromWorkbench::cur() { return dynamic_cast<PromWorkbench*>(Workbench::cur()); }


PromWorkbench::PromWorkbench() {
	canvas->add_key_listener(new IKeyListener(GDK_KEY_s, 0, on_create_script));

	win->add_tab(docBrowser = new DocBrowser(), "Doc");
	win->add_tab(scriptsForm = new ScriptsForm(), "Scripts");
	win->add_tab(createForm = new CreateForm(), "Create");

	win->add_menu("_File>__", on_import_script, win->get_menu_pos("_File>_Save as")+1);
	win->add_menu("_File>_Import script", on_import_script, win->get_menu_pos("_File>_Save as")+2);
	win->add_menu("_File>_Export script", on_export_script, win->get_menu_pos("_File>_Save as")+3);

	win->enable_menu("_File>_Export script", false);

	win->add_menu("_Create>_Script", on_create_script, win->get_menu_pos("_Create>_Module"));


	// Load config

	CSSDefinitions::add("style/basic.defs");
	CSSDefinitions::add("style/test.css");
	SVGDefinitions::add("style/svg");
	SVGDefinitions::add("style/svg2");

	ModulesLibrary::add_promethe_default_libraries();
}

void PromWorkbench::open(const std::string& filename) {
	if(project) close();
	project = new PromProject();
	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb")) {
		try {
			PromScript* s = new PromScript(filename);
			s->name = file_change_ext(file_basename(filename), "");
			PromNet* net = new PromNet();
			project->load_net(net);
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
	update();
}

void PromWorkbench::save(const std::string& filename) {
	if(!project) return;
	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb"))
		project->save_to_single_script(filename);
	else if(file_has_ext(filename, ".net"))
		project->save_net(filename);
	else
		ERROR("Can't save - unknown extension : " << filename);
	update();
}

void PromWorkbench::import() {
	std::string filename = open_file_dialog(win->widget);
	if(!filename.empty()) import(filename);
}

void PromWorkbench::export_script() {
	std::string filename = save_file_dialog(win->widget);
	if(!filename.empty()) export_script(filename);
}

void PromWorkbench::export_script(const std::string& filename) {
	GroupPromScript* gps = dynamic_cast<GroupPromScript*>(get_single_selected_module());
	if(!gps) return;
	gps->script->save(filename);
}


void PromWorkbench::import(const std::string& filename) {
	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb")) {
		try {
			PromScript* s = new PromScript(filename);
			s->name = file_change_ext(file_basename(filename), "");
			if(!project->net) { PromNet* net = new PromNet(); project->load_net(net);}
			project->net->add(new PromNode(project->net, s));
			project->add(s);
		} catch(...) {}
	} else {
		ERROR("Unknown file format " << filename);
	}
	document->update_links_layers();
	canvas->update_layers();
	canvas->zoom_all();
	update();
}

void PromWorkbench::new_document() {
	if(project) close();
	project = new PromProject();
	canvas->zoom_reset();
	update();
}

void PromWorkbench::close() {
	if(!project) return;
	delete project;
	project = 0;
	new_document();
	update();
}

void PromWorkbench::create_script() {
	canvas->start_creator(new PromScriptCreator(project));
}

void PromWorkbench::create_module() {
	canvas->start_creator(new PromGroupCreator(project));
}

void PromWorkbench::create_link() {
	canvas->start_creator(new PromLinkCreator(project));
}


void PromWorkbench::update(bool force) {
	Workbench::update(force);
	if(bPreventUpdating && !force) return;
	scriptsForm->update();
	docBrowser->update(get_selected_modules());
}



void PromWorkbench::on_selection_change() {
	Workbench::on_selection_change();
	win->enable_menu("_File>_Export script", dynamic_cast<GroupPromScript*>(get_single_selected_module())!=0 );
}

