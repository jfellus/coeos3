/*
 * PromProject.h
 *
 *  Created on: 23 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMPROJECT_H_
#define PROMPROJECT_H_

#include <creators/ModuleCreator.h>
#include <creators/LinkCreator.h>
#include "../workbench/PromWorkbench.h"
#include "promnet/PromNet.h"
#include "../modules/ModulePromGroup.h"
#include "../modules/LinkPromLink.h"
#include "../modules/GroupPromScript.h"


using namespace libboiboites;
namespace coeos {


class PromProject {
public:
	std::vector<ModulePromGroup*> groups;
	std::vector<LinkPromLink*> links;
	std::vector<GroupPromScript*> scripts;
	PromNet* net = NULL;

public:
	PromProject() {}
	~PromProject() {
		while(links.size()>0) delete links[0];
		while(groups.size()>0) delete groups[0];
		if(net) delete net;
	}

	void set_net(PromNet* net);
	void save_net();
	void save_net(const std::string& filename);
	void save_script(PromScript* script);

	void add(PromScript* script);

	inline void add(GroupPromScript* s) {scripts.push_back(s);}
	inline void add(ModulePromGroup* g) {groups.push_back(g);}
	inline void add(LinkPromLink* l) {links.push_back(l);}
	inline void remove(ModulePromGroup* g) {vector_remove(groups, g);}
	inline void remove(LinkPromLink* l) {vector_remove(links, l);}
	inline void remove(GroupPromScript* s) {vector_remove(scripts, s);}

	ModulePromGroup* get(PromGroup* g);
	LinkPromLink* get(PromLink* l);
	GroupPromScript* get(PromScript* s);

	ModulePromGroup* get_group_by_no_name(const std::string& s);



	uint get_scripts_count() { return scripts.size(); }
	uint get_modules_count() { return groups.size(); }
	uint get_links_count() { return links.size(); }

	void create_timescales_groups(GroupPromScript* script);
	void layout_scripts();
	void save_to_single_script(const std::string& filename);
	int infer_timescale(Group* g);

	void process_cross_script_link(LinkPromLink* link);
};











}


#endif /* PROMPROJECT_H_ */
