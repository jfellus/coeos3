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

class PromProject;

class PromWorkbench : public Workbench {
public:
	PromProject* project = NULL;

	ScriptsForm* scriptsForm = 0;
	DocBrowser* docBrowser = 0;
	CreateForm* createForm = 0;
	TagsForm* tagsForm = 0;


public:
	static PromWorkbench* cur();
	PromWorkbench();
	virtual ~PromWorkbench() {}


	////////
	// IO //
	////////

	virtual void new_document();
	virtual void close();
	virtual void open(const std::string& filename);
	virtual void save(const std::string& filename);
	virtual void save();
	virtual void import();
	virtual void export_script();

	virtual void import(const std::string& filename);
	virtual void export_script(const std::string& filename);


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


	/////////////
	// RUNTIME //
	/////////////

	virtual void run_project();
	virtual void stop_project();
};

#endif /* PROMWORKBENCH_H_ */
