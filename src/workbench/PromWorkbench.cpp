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
#include "../commands/CommandBlurTag.h"
#include "../commands/CommandUnblurTag.h"
#include "../promethe/launcher/Launcher.h"
#include <commands/CommandPaste.h>
#include "../commands/CommandPasteExt.h"
#include "widgets/BugTracker.h"
#include <workbench/Job.h>
#include "components/LinkLinkComponent.h"


using namespace libboiboites;
namespace coeos {



static void on_create_script() { PromWorkbench::cur()->create_script(); }
static void on_import_script() { PromWorkbench::cur()->import(); }
static void on_export_script() { PromWorkbench::cur()->export_script(); }
static void on_scale_selection(double x, double y, double dx, double dy, void* data) {PromWorkbench::cur()->scale_selection(dy);}
static void on_open_recent_document(GtkMenuItem* i, void* param) {std::string s = *((std::string*)param); PromWorkbench::cur()->open(s); }
static void on_compile() {PromWorkbench::cur()->compile();}
static void _on_launch(GtkToggleButton *togglebutton, gpointer user_data) {if(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(togglebutton))) PromWorkbench::cur()->launch_project();}
static void _on_launch2() { PromWorkbench::cur()->launch_project();}
static void _on_launch_gui(GtkToggleButton *togglebutton, gpointer user_data) {if(gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(togglebutton)))  PromWorkbench::cur()->launch_project(true);}
static void _on_stop() {PromWorkbench::cur()->stop_project(); }
static void on_edit() {PromWorkbench::cur()->edit();}
static void _on_create_one_to_one_link() {PromWorkbench::cur()->create_link(No_l_1_1_modif);}
static void on_display_links_norm()  {PromWorkbench::cur()->toggle_display_links_norm();}
static void on_display_links_name()  {PromWorkbench::cur()->toggle_display_links_name();}



PromWorkbench* PromWorkbench::cur() { return dynamic_cast<PromWorkbench*>(Workbench::cur()); }


PromWorkbench::PromWorkbench() {
	app_dir = TOSTRING(main_dir() << "/.coeos++/");
	application_name = "Coeos++";
	update_title();
	win->set_icon("coeos++.png");

	f_read_lines(TOSTRING(home() << "/.coeos++/recent.txt"),recent_documents);

	canvas->add_key_listener(new IKeyListener(GDK_KEY_s, 0, on_create_script));

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

	win->add_menu("_View>Display links norm", on_display_links_norm);
	win->add_menu("_View>Display links name", on_display_links_name);


	win->add_toolbar("new script", "style/icons/new_script.gif", on_create_script, win->get_toolbar_pos("new module"));
	win->add_toolbar("import script",  "style/icons/import_script.gif", on_import_script, win->get_toolbar_pos("undo")-1);
	win->add_toolbar("export script", "style/icons/export_script.gif", on_export_script, win->get_toolbar_pos("undo")-1);
	win->add_toolbar("__");
	win->add_toolbar("edit", "style/icons/edit.gif", on_edit);
	win->add_toolbar("compile", "style/icons/compile.gif", on_compile);
	win->add_toolbar_toggle("launch", "style/icons/play.gif", _on_launch);
	win->add_toolbar_toggle("launch_gui", "style/icons/play_gui.gif", _on_launch_gui);
	win->add_toolbar("stop", "style/icons/stop.gif", _on_stop);


	canvas->add_key_listener(new IKeyListener(GDK_KEY_F5, 0, _on_launch2));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_F6, 0, _on_stop));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_c, 0, on_compile));
	canvas->add_key_listener(new IKeyListener(GDK_KEY_o, 0, _on_create_one_to_one_link));

	canvas->add_scroll_listener(new IScrollListener(GDK_CONTROL_MASK|GDK_SUPER_MASK, coeos::on_scale_selection, NULL));

	// Load config

	CSSDefinitions::add(TOSTRING(app_dir << "style/basic.defs"));
	CSSDefinitions::add(TOSTRING(app_dir << "style/test.css"));
	SVGDefinitions::add(TOSTRING(app_dir << "style/svg"));

	ModulesLibrary::add_promethe_default_libraries();
}
void PromWorkbench::update_recent_menu() {
	while(win->get_menu("_File>_Export script",1)!="__")
		win->remove_menu("_File>_Export script",1);
	for(uint i=0; i<MIN(9,recent_documents.size()); i++)
		win->add_menu(TOSTRING("_File>_" << i+1 << " - " << recent_documents[i]).c_str(),
				on_open_recent_document, &recent_documents[i], win->get_menu_pos("_File>_Export script")+i+2);
}


int _zoom_all(void* p) {
	PromWorkbench* w = (PromWorkbench*)p;
	w->canvas->zoom_all();
	return FALSE;
}
void PromWorkbench::do_open(const std::string& _filename) {
	canvas->OFF();

	std::string filename = _filename;
	if(project) do_close();
	project = new PromProject();

	vector_remove(recent_documents, filename);
	recent_documents.insert(recent_documents.begin(), filename);
	f_write_lines(TOSTRING(home() << "/.coeos++/recent.txt"),recent_documents);
	update_recent_menu();

	DBG("Open " << filename);

	if(file_has_ext(filename, ".script") || file_has_ext(filename, ".symb")) {
		try {
			PromScript* s = new PromScript(filename);
			s->name = file_change_ext(file_basename(filename), "");
			project->set_net(new PromNet());
			project->net->add(new PromNode(project->net, s));
			project->add(s);
			document->open(filename);
			update_title();
		} catch(...) {}
	} else if(file_has_ext(filename, ".net")) {
		project->set_net(new PromNet(filename));
		//project->layout_scripts();
		document->open(filename);
		update_title();
	} else {
		ERROR("Unknown file format " << filename);
	}
	document->update_links_layers();
	canvas->update_layers();
	canvas->ON();
	update();
	g_timeout_add(10, _zoom_all, this);
}

void PromWorkbench::save() {
	if(!project) return;
	if(!project->net) return;
	if(project->net->filename.empty()) PromWorkbench::cur()->save_as();
	else JOB_SUBMIT("Save", PromWorkbench::cur()->project->save_net());
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

void PromWorkbench::export_selection_as_script(const std::string& filename) {
	create_dir_for(filename);
	PromScript* s = new PromScript();
	std::map<PromGroup*,PromGroup*> copies;
	for(uint i=0; i<get_selected_modules_count(); i++) {
		ModulePromGroup* m = dynamic_cast<ModulePromGroup*>((*get_selected_modules())[i]);
		if(!m) continue;
		PromGroup* c = m->group->copy();
		copies[m->group] = c;
		s->add_group(c);
	}
	for(uint i=0; i<get_selected_links_count(); i++) {
		LinkPromLink* l = dynamic_cast<LinkPromLink*>((*get_selected_links())[i]);
		if(!l) continue;
		if(!l->dst->is_selected() || !l->src->is_selected()) continue;
		PromLink* c = l->link->copy();
		c->src = copies[c->src];
		c->dst = copies[c->dst];
		s->add_link(c);
	}
	s->save_as(filename);
	for(auto i = copies.begin(); i != copies.end(); i++) {
		(*i).first->script->remove_group((*i).second, true);
	}
	delete s;
}


void PromWorkbench::export_script(const std::string& filename) {
	GroupPromScript* gps = dynamic_cast<GroupPromScript*>(get_single_selected_module());
	if(!gps) return;
	gps->script->save_as(filename);
}

void PromWorkbench::save_script(const std::string& script_name) {
	GroupPromScript* gps = project->get_script_by_name(script_name);
	if(!gps) return;
	gps->script->save();
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
	unselect_all();
	properties->reset();
	if(project) {delete project;	project = 0;}
	canvas->clear();

	STATUS("New network");

	project= new PromProject();
	project->set_net(new PromNet());
	document->new_document();

	canvas->zoom_reset();
	document->update_links_layers();
	canvas->update_layers();
	canvas->ON();
	canvas->repaint();
	update_title();
	update();
}

void PromWorkbench::do_close() {
	do_new_document();
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

void PromWorkbench::create_link(int type) {
	canvas->start_creator(new PromLinkCreator(project, type));
}


void PromWorkbench::do_update() {
	Workbench::do_update();
	scriptsForm->update();
	docBrowser->update(get_selected_modules());
	tagsForm->update();
}


void PromWorkbench::compile() {
	if(!project) return;
	if(!project->net) return;
	if(project->net->filename.empty()) {
		if(question("You must save your project first. Do you wish to do it now ?")) save_as();
		else return;
	} else save();
	if(project->net->filename.empty()) return;
	JOB_SUBMIT("Compile", {PromWorkbench::cur()->project->compile();});
}


static bool _something_launched = false;
static int _update_launch_toolbar(void* p) {
	PromWorkbench* w = (PromWorkbench*)p;
	if(!_something_launched) {
		w->win->enable_toolbar("launch", true);
		w->win->enable_toolbar("launch_gui", true);
	} else {
		w->win->enable_toolbar("launch", false);
		w->win->enable_toolbar("launch_gui", false);
	}
	return FALSE;
}

void PromWorkbench::on_start(PromNodeThread* t) {
	_something_launched = true;
	g_timeout_add(1, _update_launch_toolbar, this);
	scriptsForm->update();
	POPUP("Started " << t->node->script->name);
}

void PromWorkbench::on_stop(PromNodeThread* t) {
	_something_launched = false;
	g_timeout_add(1, _update_launch_toolbar, this);
	scriptsForm->update();
	POPUP("Stopped " << t->node->script->name);
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

void PromWorkbench::blur_tag(const std::string& tagname) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(t->bBlur) return;
	DBG("BLUR " << tagname);
	(new CommandBlurTag(Document::cur(),tagname))->execute();
}

void PromWorkbench::unblur_tag(const std::string& tagname) {
	Tag* t = Tags::get(tagname);
	if(!t) {ERROR("Tag " << tagname << " doesn't exist"); return;}
	if(!t->bBlur) return;
	DBG("UNBLUR " << tagname);
	(new CommandUnblurTag(Document::cur(),tagname))->execute();
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

void PromWorkbench::launch_project(bool bGui) {
	if(!project) return;
	Launcher::start(project, bGui);
}

void PromWorkbench::stop_project() {
	if(!project) return;
	Launcher::stop(project);
}

void PromWorkbench::compile_script(const std::string& script_name) {
	if(!project) return;
	PromNode* n = project->net->get_node_by_name(script_name);
	if(!n) return;
	Compiler::compile(n);
}

void PromWorkbench::launch_script(const std::string& script_name, bool bGui) {
	if(!project) return;
	PromNode* n = project->net->get_node_by_name(script_name);
	if(!n) return;
	Launcher::start(n, bGui);
}

void PromWorkbench::stop_script(const std::string& script_name) {
	if(!project) return;
	PromNode* n = project->net->get_node_by_name(script_name);
	if(!n) return;
	Launcher::stop(n);
}



static long copy_time = 0;
void PromWorkbench::cut() {
	shell("rm -f ~/.leto/copy_buffer.script; mkdir -p ~/.leto/");
	Workbench::cut();
	export_selection_as_script(TOSTRING(home() << "/.leto/copy_buffer.script"));
	copy_time = file_get_modification_time(TOSTRING(home() << "/.leto/copy_buffer.script"));
}

void PromWorkbench::copy() {
	shell("rm -f ~/.leto/copy_buffer.script; mkdir -p ~/.leto/");
	Workbench::copy();
	export_selection_as_script(TOSTRING(home() << "/.leto/copy_buffer.script"));
	copy_time = file_get_modification_time(TOSTRING(home() << "/.leto/copy_buffer.script"));
}

void PromWorkbench::paste() {
	if(file_exists(TOSTRING(home() << "/.leto/copy_buffer.script")) && abs(copy_time - file_get_modification_time(TOSTRING(home() << "/.leto/copy_buffer.script")))>2) {
		(new CommandPasteExt(canvas->mousePosDoc.x, canvas->mousePosDoc.y, TOSTRING(home() << "/.leto/copy_buffer.script")))->execute();
	} else	Workbench::paste();
}




/////////////
// DISPLAY //
/////////////

void PromWorkbench::toggle_display_links_norm() {
	libboiboites::LinkComponentStyle::bText2_force = !libboiboites::LinkComponentStyle::bText2_force;
	canvas->repaint();
}

void PromWorkbench::toggle_display_links_name() {
	libboiboites::LinkComponentStyle::bText_force = !libboiboites::LinkComponentStyle::bText_force;
	canvas->repaint();
}


}
