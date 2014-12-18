/*
 * PromWorkbench.h
 *
 *  Created on: 22 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMWORKBENCH_H_
#define PROMWORKBENCH_H_

#include <workbench/Workbench.h>
#include "widgets/ScriptsForm.h"
#include "widgets/DocBrowser.h"
#include "widgets/CreateForm.h"
#include "widgets/TagsForm.h"
#include "widgets/BugTracker.h"

using namespace libboiboites;
namespace coeos {

class PromProject;
class GroupPromScript;

class PromWorkbench : public Workbench {
public:
	PromProject* project = NULL;

	ScriptsForm* scriptsForm = 0;
	DocBrowser* docBrowser = 0;
	CreateForm* createForm = 0;
	TagsForm* tagsForm = 0;
	BugTracker* bugTracker = 0;

	std::vector<std::string> recent_documents;

public:
	static PromWorkbench* cur();
	PromWorkbench();
	virtual ~PromWorkbench() {}


	////////
	// IO //
	////////

	virtual void save();
	virtual void import();
	virtual void export_script();
	virtual void export_selection_as_script(const std::string& filename);

	virtual void do_new_document();
	virtual void do_close();
	virtual void do_open(const std::string& filename);
	virtual void do_save(const std::string& filename);

	virtual void import(const std::string& filename);
	virtual void export_script(const std::string& filename);


	virtual void cut();
	virtual void copy();
	virtual void paste();

	/////////////
	// METHODS //
	/////////////

	virtual void create_script();
	virtual void create_module();
	virtual void create_module(const std::string& nametype);
	virtual void create_link();

	virtual void update(bool force = false);


	////////////
	// EVENTS //
	////////////

	virtual void on_selection_change();
	virtual void scale_selection(double amount);


	/////////////
	// SCRIPTS //
	/////////////

	virtual void set_script_property(const std::string& scriptname, const std::string& key, const std::string& value);

	GroupPromScript* get_script_at(double x, double y);

	//////////
	// TAGS //     // TODO Perhaps move to libboiboites
	//////////

	virtual void tag_selection(const std::string& tagname);
	virtual void untag_selection(const std::string& tagname);
	virtual void isolate_tag(const std::string& tagname, bool toggle = true);
	virtual void unisolate_tag();
	virtual void tag_set_css(const std::string& tagname, const std::string& css);
	virtual void add_tag(const std::string& tagname);
	virtual void remove_tag(const std::string& tagname);
	virtual void show_tag(const std::string& tagname);
	virtual void hide_tag(const std::string& tagname);
	virtual void lock_tag(const std::string& tagname);
	virtual void unlock_tag(const std::string& tagname);
	virtual void change_tag_name(const std::string& oldname, const std::string& newname);
	virtual void blur_tag(const std::string& tagname);
	virtual void unblur_tag(const std::string& tagname);


	/////////////
	// RUNTIME //
	/////////////

	virtual void run_project();
	virtual void stop_project();

protected:
	void update_recent_menu();
};


}

#endif /* PROMWORKBENCH_H_ */
