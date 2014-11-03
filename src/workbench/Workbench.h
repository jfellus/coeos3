/*
 * Workbench.h
 *
 *  Created on: 12 oct. 2014
 *      Author: jfellus
 */

#ifndef COEOS_WORKBENCH_H_
#define COEOS_WORKBENCH_H_

#include "../widget/Window.h"
#include "../module/Document.h"
#include "../util/gui.h"
#include "../components/ChangeGroupCreator.h"

void set_status(const std::string& text);
#define STATUS(x) set_status(TOSTRING(x))

class Workbench : public IModuleSelectionListener, ILinkSelectionListener, IPropertiesListener {
public:
	Document* document = 0;

	Window* win = 0;
		ZoomableDrawingArea* canvas;
		DocBrowser* docBrowser = 0;
		PropertiesForm* properties = 0;
		InfoForm* infoform = 0;

	std::vector<Module*> selected_modules;
	std::vector<Link*> selected_links;

	static Workbench* cur();
public:
	Workbench();
	virtual ~Workbench();


	void unselect_all() {
		canvas->unselect_all();
		while(selected_modules.size()>0) vector_remove(selected_modules, selected_modules[0]);
		while(selected_links.size()>0) vector_remove(selected_links, selected_links[0]);
	}
	void select(Module* m) {m->select();}


	void start_change_group() {
		canvas->start_creator(new ChangeGroupCreator());
	}

	virtual void on_module_selected(Module* m, bool bSelected) {
		if(!canvas->isSelecting) properties->update(&selected_modules);

		if(bSelected) selected_modules.push_back(m);
		else vector_remove(selected_modules, m);

		if(!canvas->isSelecting) update();
	}

	virtual void update() {
		STATUS(selected_modules.size() << " modules selected\t\t" << selected_links.size() << " links selected");
		properties->update(&selected_modules);
		docBrowser->update(selected_modules);
		if(infoform) infoform->update();
		canvas->grab_focus();
		canvas->repaint();
	}

	virtual void on_link_selected(Link* m, bool bSelected) {
		if(bSelected) selected_links.push_back(m);
		else vector_remove(selected_links, m);
		if(!canvas->isSelecting) update();
	}

	virtual void new_document() = 0;
	virtual void open(const std::string& filename) = 0;
	virtual void save(const std::string& filename) = 0;

	virtual void close() = 0;
	virtual void create_module() = 0;
	virtual void create_link() = 0;

	void change_group_selected() {
		if(!selected_modules.empty()) canvas->start_creator(new ChangeGroupCreator());
	}

	void group_selection() {
		if(selected_modules.size()==1 && dynamic_cast<Group*>(selected_modules[0])!=NULL) {
			ungroup_selected();
		} else {
			document->group_selection();
			canvas->update_layers();
		}
	}

	void ungroup_selected() {
		document->ungroup_selected();
		canvas->update_layers();
	}

	void open_dialog() {
		std::string filename = open_file_dialog(win->widget);
		if(!filename.empty()) open(filename);
	}

	void save_dialog() {
		std::string filename = save_file_dialog(win->widget);
		if(!filename.empty()) {
			while(file_exists(filename)) {
				if(win->yes_no_box(TOSTRING("File " << filename << " exists. Overwrite ?"))) break;
				filename = save_file_dialog(win->widget);
				if(filename.empty()) break;
			}
			if(!filename.empty()) save(filename);
		}
	}


	void run();

	virtual void on_property_change(Module* m, const std::string& name, const std::string& val) {
		canvas->repaint();
	}


	void toggle_selection_tag(int i) {
		Document::cur()->toggle_selection_tag(i);
		canvas->repaint();
	}

	void add_selection_tag(int i) {
		Document::cur()->add_selection_tag(i);
		canvas->repaint();
	}

	void remove_selection_tag(int i) {
		Document::cur()->remove_selection_tag(i);
		canvas->repaint();
	}

	virtual void dump(std::ostream& os) {
		os << "Workbench";
	}
	friend std::ostream& operator<<(std::ostream& os, Workbench* a);
};

std::ostream& operator<<(std::ostream& os, Workbench* a);

#endif /* COEOS_WORKBENCH_H_ */
