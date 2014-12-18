/*
 * CommandPasteExt.h
 *
 *  Created on: 12 nov. 2014
 *      Author: jfellus
 */

#ifndef CommandPasteExt_H_
#define CommandPasteExt_H_

#include <commands/Command.h>
#include <module/Document.h>
#include <ZoomableDrawingArea.h>
#include "../modules/GroupPromScript.h"
#include "../workbench/PromWorkbench.h"

using namespace libboiboites;


namespace coeos {


class CommandPasteExt : public Command {
public:
	Document* doc;
	double x,y;
	std::string filename;
	Group* group = NULL;
	GroupPromScript* gps = NULL;

	std::vector<Module*> clipboard_modules;
	std::vector<Link*> clipboard_links;

public:

	CommandPasteExt(double x, double y, const std::string& buffer_script_file) : doc(Document::cur()), x(x), y(y), filename(buffer_script_file) {
		 group = doc->get_group_at(x,y);
		 if(group) gps = PromWorkbench::cur()->project->get_owner_script(group);

		 Vector2D cnt;

		 if(gps) {
			 PromScript *s = new PromScript(filename);
			 for(uint i=0; i<s->groups.size(); i++) {
				 PromGroup* g = s->groups[i];
				 g->no_name = gps->script->new_noname();
				 gps->script->add_group(g);
				 g->project = gps->project;
				 ModulePromGroup* mpg = new ModulePromGroup(g);
				 gps->project->add(mpg);
				 group->add(mpg);
				 clipboard_modules.push_back(mpg);

				 cnt += mpg->component->center();
			 }
			 cnt /= clipboard_modules.size();

			 for(uint i=0; i<s->links.size(); i++) {
				 PromLink* l = s->links[i];
				 l->project = gps->project;
				 gps->script->add_link(l);
				 LinkPromLink* lpl = new LinkPromLink(l);
				 gps->project->add(lpl);
				 clipboard_links.push_back(lpl);
				 lpl->detach();
			 }

			 Vector2D d(x-cnt.x, y-cnt.y);
			 for(uint i=0; i<clipboard_modules.size(); i++) {
				 Vector2D c =  clipboard_modules[i]->component->center();
				 clipboard_modules[i]->component->center(c + d);
				 clipboard_modules[i]->detach();
			 }

			 s->groups.clear();
			 s->links.clear();
			 delete s;


		 } else PromWorkbench::cur()->import(filename);
	}

	virtual ~CommandPasteExt() {}

	virtual void execute() {
		for(uint i=0; i<clipboard_modules.size(); i++) clipboard_modules[i]->attach();
		for(uint i=0; i<clipboard_links.size(); i++) clipboard_links[i]->attach();

		doc->update_links_layers();
		doc->fire_change_event();
	}

	virtual void undo() {
		for(uint i=0; i<clipboard_modules.size(); i++) clipboard_modules[i]->detach();
		for(uint i=0; i<clipboard_links.size(); i++) clipboard_links[i]->detach();
		doc->update_links_layers();
		doc->fire_change_event();
	}

	virtual std::string get_text() {
		return "Paste";
	}
};

}

#endif /* CommandPasteExt_H_ */
