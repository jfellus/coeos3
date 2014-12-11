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
#include "widgets/BugTracker.h"
#include <workbench/Job.h>

using namespace libboiboites;
namespace coeos {



static void on_create_script() { PromWorkbench::cur()->create_script(); }
static void on_import_script() { PromWorkbench::cur()->import(); }
static void on_export_script() { PromWorkbench::cur()->export_script(); }
static void on_run() {PromWorkbench::cur()->run_project(); }
static void on_stop() {PromWorkbench::cur()->stop_project(); }
static void on_scale_selection(double x, double y, double dx, double dy) {PromWorkbench::cur()->scale_selection(dy);}
static void on_copy() {PromWorkbench::cur()->copy(); }
static void on_paste() {PromWorkbench::cur()->paste(); }
static void on_cut() {PromWorkbench::cur()->cut(); }
static void on_open_recent_document(GtkMenuItem* i, void* param) {std::string s = *((std::string*)param); PromWorkbench::cur()->open(s); }

PromWorkbench* PromWorkbench::cur() { return dynamic_cast<PromWorkbench*>(Workbench::cur()); }


PromWorkbench::PromWorkbench() {
	f_read_lines(TOSTRING(home() << "/.coeos++/recent.txt"),recent_documents);

	canvas->add_key_listener(new IKeyListener(GDK_KEY_s, 0, on_create_script));

	win->set_title("Coeos++");

	win->add_tab(docBrowser = new DocBrowser(), "Doc");
	win->add_tab(scriptsForm = new ScriptsForm(), "Scripts");
	win->add_tab(createForm = new CreateForm(), "Create");
	win->add_tab(tagsForm = new TagsForm(), "Tags");
	win->add_tab(bugTracker = new BugTracker(), "Bugs");


	win->add_menu("_File>__", on_import_script, win->get_menu_pos("_File>_Save as")+1);
	win->add_menu("_File>_Import script", on_import_script, win->get_menu_pos("_File>_Save as")+2);
	win->add_menu("_File>_Export script", on_export_script, win->get_menu_pos("_File>_Save as")+3);

	win->add_menu("_File>__", on_export_script, win->get_menu_pos("_File>_Export script")+1);
	update_recent_menu();

	win->enable_menu("_File>_Export script", false);

	win->add_menu("_Create>_Script", on_create_script, win->get_menu_pos("_Create>_Module"));


	canvas->add_key_listener(new IKeyListener(GDK_KEY_F5, 0, on_run));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_F6, 0, on_stop));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_v, GDK_CONTROL_MASK, on_paste));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_c, GDK_CONTROL_MASK, on_copy));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_x, GDK_CONTROL_MASK, on_cut));


	canvas->add_scroll_listener(new IScrollListener(GDK_CONTROL_MASK|GDK_SUPER_MASK, coeos::on_scale_selection));

	// Load config

	CSSDefinitions::add("style/basic.defs");
	CSSDefinitions::add("style/test.css");
	SVGDefinitions::add("style/svg");

	ModulesLibrary::add_promethe_default_libraries();
}
void PromWorkbench::update_recent_menu() {
	while(win->get_menu("_File>_Export script",1)!="__")
		win->remove_menu("_File>_Export script",1);
	for(uint i=0; i<MIN(9,recent_documents.size()); i++)
		win->add_menu(TOSTRING("_File>_" << i+1 << " - " << recent_documents[i]).c_str(),
				on_open_recent_document, &recent_documents[i], win->get_menu_pos("_File>_Export script")+i+2);
}

void PromWorkbench::do_open(const std::string& _filename) {
	canvas->OFF();
	if(project) do_close();
	std::string filename = _filename;
	project = new PromProject();

	vector_remove(recent_documents, filename);
	recent_documents.insert(recent_documents.begin(), filename);
	f_write_lines(TOSTRING(home() << "/.coeos++/recent.txt"),recent_documents);
	update_recent_menu();

	STATUS("Open " << filename);

	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb")) {
		try {
			PromScript* s = new PromScript(filename);
			s->name = file_change_ext(file_basename(filename), "");
			project->set_net(new PromNet());
			project->net->add(new PromNode(project->net, s));
			project->add(s);
			set_title(TOSTRING("Coeos++ - " << filename));
		} catch(...) {}
	} else if(file_has_ext(filename, ".net")) {
		project->set_net(new PromNet(filename));
		//project->layout_scripts();
		set_title(TOSTRING("Coeos++ - " << filename));
	} else {
		ERROR("Unknown file format " << filename);
	}
	document->update_links_layers();
	canvas->update_layers();
	canvas->zoom_all();
	canvas->ON();
	update();
}

void PromWorkbench::save() {
	JOB_SUBMIT("Save", PromWorkbench::cur()->project->save_net());
}

void PromWorkbench::do_save(const std::string& filename) {
	if(!project) return;
	canvas->OFF();

	recent_documents.insert(recent_documents.begin(), filename);
	f_write_lines(TOSTRING(home() << "/.coeos++/recent.txt"),recent_documents);

	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb"))
		project->save_to_single_script(filename);
	else if(file_has_ext(filename, ".net"))
		project->save_net(filename);
	else
		ERROR("Can't save - unknown extension : " << filename);
	canvas->ON();
	DBG("Saved !");
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
	canvas->OFF();
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
	canvas->ON();
	update();
}

void PromWorkbench::do_new_document() {
	canvas->OFF();
	if(project) close();
	STATUS("New network");
	project = new PromProject();
	canvas->zoom_reset();
	canvas->ON();
	update();
}

void PromWorkbench::do_close() {
	canvas->OFF();
	unselect_all();
	properties->reset();
	if(!project) return;
	delete project;
	canvas->clear();
	project = 0;
	do_new_document();
	canvas->ON();
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

GroupPromScript* PromWorkbench::get_script_at(double x, double y) {
	if(!project) return NULL;
	Module* g = Document::cur()->get_group_at(x,y);
	while(g!=NULL && !dynamic_cast<GroupPromScript*>(g)) { g = g->parent; }
	return dynamic_cast<GroupPromScript*>(g);
}

void PromWorkbench::run_project() {
	if(!project) return;
	Launcher::start(project);
}

void PromWorkbench::stop_project() {
	if(!project) return;
	Launcher::stop(project);
}

}
