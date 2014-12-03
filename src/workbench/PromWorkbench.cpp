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
#include "../commands/CommandIsolateTag.h"
#include "../commands/CommandTagSelection.h"
#include "../commands/CommandTagSetCSS.h"
#include "../commands/CommandUnisolateTag.h"
#include "../commands/CommandUntagSelection.h"
#include "../commands/CommandCreateTag.h"
#include "../commands/CommandDeleteTag.h"
#include "../commands/CommandHideTag.h"
#include "../commands/CommandShowTag.h"
#include "../commands/CommandLockTag.h"
#include "../commands/CommandUnlockTag.h"
#include "../commands/CommandSetTagName.h"
#include "../commands/CommandScriptSetProperty.h"
#include "../promethe/launcher/Launcher.h"



static void on_create_script() { PromWorkbench::cur()->create_script(); }
static void on_import_script() { PromWorkbench::cur()->import(); }
static void on_export_script() { PromWorkbench::cur()->export_script(); }
static void on_run() {PromWorkbench::cur()->run_project(); }
static void on_stop() {PromWorkbench::cur()->stop_project(); }
static void on_scale_selection(double x, double y, double dx, double dy) {PromWorkbench::cur()->scale_selection(dy);}

PromWorkbench* PromWorkbench::cur() { return dynamic_cast<PromWorkbench*>(Workbench::cur()); }


PromWorkbench::PromWorkbench() {
	canvas->add_key_listener(new IKeyListener(GDK_KEY_s, 0, on_create_script));

	win->add_tab(docBrowser = new DocBrowser(), "Doc");
	win->add_tab(scriptsForm = new ScriptsForm(), "Scripts");
	win->add_tab(createForm = new CreateForm(), "Create");
	win->add_tab(tagsForm = new TagsForm(), "Tags");

	win->add_menu("_File>__", on_import_script, win->get_menu_pos("_File>_Save as")+1);
	win->add_menu("_File>_Import script", on_import_script, win->get_menu_pos("_File>_Save as")+2);
	win->add_menu("_File>_Export script", on_export_script, win->get_menu_pos("_File>_Save as")+3);

	win->enable_menu("_File>_Export script", false);

	win->add_menu("_Create>_Script", on_create_script, win->get_menu_pos("_Create>_Module"));


	canvas->add_key_listener(new IKeyListener(GDK_KEY_F5, 0, on_run));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_F6, 0, on_stop));

	canvas->add_scroll_listener(new IScrollListener(GDK_CONTROL_MASK|GDK_SHIFT_MASK, ::on_scale_selection));

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
			project->set_net(new PromNet());
			project->net->add(new PromNode(project->net, s));
			project->add(s);
		} catch(...) {}
	} else if(file_has_ext(filename, ".net")) {
		project->set_net(new PromNet(filename));
		//project->layout_scripts();
	} else {
		ERROR("Unknown file format " << filename);
	}
	document->update_links_layers();
	canvas->update_layers();
	canvas->zoom_all();
	update();
}

void PromWorkbench::save() {
	project->save_net();
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
	gps->script->save_as(filename);
}


void PromWorkbench::import(const std::string& filename) {
	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb")) {
		try {
			PromScript* s = new PromScript(filename);
			s->name = file_change_ext(file_basename(filename), "");
			if(!project->net) project->set_net(new PromNet());
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

void PromWorkbench::create_module(const std::string& nametype) {
	canvas->start_creator(new PromGroupCreator(project, nametype));
}

void PromWorkbench::create_link() {
	canvas->start_creator(new PromLinkCreator(project));
}


void PromWorkbench::update(bool force) {
	set_title(TOSTRING("Coeos++ - " << project->net->filename));
	Workbench::update(force);
	if(bPreventUpdating && !force) return;
	scriptsForm->update();
	docBrowser->update(get_selected_modules());
	tagsForm->update();
}



void PromWorkbench::on_selection_change() {
	Workbench::on_selection_change();
	win->enable_menu("_File>_Export script", dynamic_cast<GroupPromScript*>(get_single_selected_module())!=0 );
}

void PromWorkbench::scale_selection(double amount) {
	amount=1-amount*0.04;
	for(uint i=0; i<get_selected_modules()->size(); i++) {
		ModulePromGroup* m = dynamic_cast<ModulePromGroup*>((*get_selected_modules())[i]);
		if(!m) continue;
		m->scale(amount);
	}
	for(uint i=0; i<get_selected_links()->size(); i++) {
		LinkPromLink* l = dynamic_cast<LinkPromLink*>((*get_selected_links())[i]);
		if(!l) continue;
		l->scale(amount);
	}
}


void PromWorkbench::tag_selection(const std::string& tagname) {
	(new CommandTagSelection(Document::cur(),tagname))->execute();
}

void PromWorkbench::untag_selection(const std::string& tagname) {
	(new CommandUntagSelection(Document::cur(),tagname))->execute();
}



void PromWorkbench::isolate_tag(const std::string& tagname, bool toggle) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(t==Tags::isolated && toggle) (new CommandUnisolateTag())->execute();
	else (new CommandIsolateTag(Document::cur(), tagname))->execute();
}

void PromWorkbench::unisolate_tag() {
	if(Tags::isolated == NULL) return;
	(new CommandUnisolateTag())->execute();
}

void PromWorkbench::tag_set_css(const std::string& tagname, const std::string& css) {
	(new CommandTagSetCSS(tagname, css))->execute();
}

void PromWorkbench::add_tag(const std::string& tagname) {
	if(Tags::get(tagname)) {ERROR("Tag " << tagname << " already exists"); return;}
	(new CommandCreateTag(tagname))->execute();
}

void PromWorkbench::remove_tag(const std::string& tagname) {
	if(!Tags::get(tagname)) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	(new CommandDeleteTag(tagname))->execute();
}

void PromWorkbench::show_tag(const std::string& tagname) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(t->bVisible) return;
	(new CommandShowTag(tagname))->execute();
}

void PromWorkbench::hide_tag(const std::string& tagname) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(!t->bVisible) return;
	(new CommandHideTag(tagname))->execute();
}

void PromWorkbench::lock_tag(const std::string& tagname) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(t->bLock) return;
	(new CommandLockTag(tagname))->execute();
}

void PromWorkbench::unlock_tag(const std::string& tagname) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(!t->bLock) return;
	(new CommandUnlockTag(tagname))->execute();
}

void PromWorkbench::change_tag_name(const std::string& oldname, const std::string& newname) {
	Tag* t = Tags::get(oldname);
	if(!t) {ERROR("Tag " << oldname << " doesn't exist"); return;}
	(new CommandSetTagName(t,newname))->execute();
}



void PromWorkbench::set_script_property(const std::string& scriptname, const std::string& key, const std::string& value) {
	if(!project) return;
	if(!project->net) return;
	PromNode* n = project->net->get_node_by_name(scriptname);
	if(!n) {ERROR("Script " << scriptname << "doesn't exist"); return;}
	(new CommandScriptSetProperty(n,key,value))->execute();
}



void PromWorkbench::run_project() {
	if(!project) return;
	Launcher::start(project);
}

void PromWorkbench::stop_project() {
	if(!project) return;
	Launcher::stop(project);
}

