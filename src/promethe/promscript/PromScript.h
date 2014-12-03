/*
 * PromScript.h
 *
 *  Created on: 18 oct. 2014
 *      Author: jfellus
 */

#ifndef PROMSCRIPT_H_
#define PROMSCRIPT_H_

#include <util/utils.h>
#include <exception>
#include <locale.h>
#include <fstream>
#include "PromGroup.h"
#include "PromLink.h"
#include "../promtools.h"



class Exception_GroupNotFound : public BasicException {
public:
	Exception_GroupNotFound(const std::string& g) : BasicException("Can't find group : ", g.c_str()) {}
};

class PromProject;
class PromNode;

class PromScript {
public:
	std::vector<PromGroup*> groups;
	std::vector<PromLink*> links;

	std::string name;
	std::string comments_groups, comments_links;

	std::string filename;

	PromProject* project = 0;
	PromNode* node = 0;

public:
	PromScript() {}
	PromScript(const std::string& filename) {load(filename);}
	virtual ~PromScript();

	PromGroup* get_group_by_no_name(const std::string& no_name) {
		for(uint i=0; i<groups.size(); i++) if(groups[i]->no_name == no_name) return groups[i];
		return NULL;
	}


	void add_group(PromGroup* group) { groups.push_back(group); group->script = this; }
	void add_link(PromLink* link) {
		if(link->src==NULL || link->dst==NULL) return;
		links.push_back(link);
		link->script = this;
	}

	void remove_group(PromGroup* group, bool remove_associated_links = true) {
		vector_remove(groups, group);
		if(remove_associated_links)
			for(uint i=0; i<links.size(); i++) {
				if(links[i]->src == group || links[i]->dst == group) remove_link(links[i--]);
			}
	}

	void remove_link(PromLink* link) {
		vector_remove(links, link);
	}

	int get_highest_timescale() {
		int max = 0;
		for(uint i=0; i<groups.size(); i++) {
			if(groups[i]->time_scale > max) max = groups[i]->time_scale;
		}
		return max;
	}

	void load(const std::string& filename);

	void save();

	void save_as(const std::string& filename) {
		this->filename = filename;
		create_dir_for(filename);
		std::ofstream f(filename);
		if(!f) DBG("Failed creating file " << filename);
		if(!write(f)) DBG("Failed writing " << filename);
		f.close();
	}

	bool read(std::istream& f) {
		try {
			comments_groups = f_read_comments(f);
			uint nb_groups; f_try_read(f, "nombre de groupes = %u\n", nb_groups);
			for(uint i=0; i<nb_groups; i++) add_group(new PromGroup(this, f));

			comments_links = f_read_comments(f);
			uint nb_links; f_try_read(f, "nombre de liaisons = %u\n", nb_links);
			for(uint i=0; i<nb_links; i++) {
				PromLink* l = new PromLink(this, f);
				if(!l->src || !l->dst) delete l;
				else add_link(l);
			}

			return true;
		} catch(const char* s) {
			fcomeBackToLine(f, -10);
			std::string ss = fgetlines(f, 10);
			ERROR("Can't read " << s);
			DBG(ss);
		return false;}
		catch(std::exception& s) { ERROR(s.what()); return false;}
	}

	bool write(std::ostream& f) {
		try {
			f_write_comments(f,comments_groups);
			f << "nombre de groupes = "<< groups.size() << "\n";
			for(uint i=0; i<groups.size(); i++) groups[i]->write(f);

			f_write_comments(f,comments_links);
			f << "nombre de liaisons = "<< links.size() << "\n";
			for(uint i=0; i<links.size(); i++) links[i]->write(f);

			return true;
		} catch(const char* s) { ERROR("Can't write " << s); return false;}
		catch(std::exception& s) { ERROR(s.what()); return false;}
	}

	virtual void dump(std::ostream& os) {
		os << "PromScript(" << name << " : " << groups.size() << " groups, " << links.size() << " links)";
	}
	friend std::ostream& operator<<(std::ostream& os, PromScript* a);
};

std::ostream& operator<<(std::ostream& os, PromScript* a);



#endif /* PROMSCRIPT_H_ */
