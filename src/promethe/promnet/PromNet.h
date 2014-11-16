/*
 * PromNet.h
 *
 *  Created on: 6 nov. 2014
 *      Author: jfellus
 */

#ifndef PROMNET_H_
#define PROMNET_H_

#include <util/utils.h>
#include "PromNode.h"


namespace xmlpp {class Element;}
class PromProject;

class PromNet {
public:
	std::vector<PromNode*> nodes;
	PromProject* project = NULL;

	std::string filename;
	std::string path_config;

public:
	PromNet() {}
	PromNet(const std::string& filename) {
		read(filename);
	}
	virtual ~PromNet() {
		while(!nodes.empty()) delete(nodes[0]);
	}

	void add(PromNode* n) {nodes.push_back(n);}
	void remove(PromNode* n) {vector_remove(nodes, n);}

	void read(const std::string& filename);
	void write(const std::string& filename);
	inline void save(const std::string& filename) {write(filename);}

	PromScript* get_script_by_name(const std::string& name) {
		for(uint i=0; i<nodes.size(); i++) {
			if(!nodes[i]->script) continue;
			if(nodes[i]->script->name == name) return nodes[i]->script;
		}
		return NULL;
	}

	inline std::string get_dir() {return file_dirname(filename); }

	void realize();
protected:
	void read_network(xmlpp::Element* node);
	void read_link_list(xmlpp::Element* node);
	void write_network(xmlpp::Element* node);
	void write_link_list(xmlpp::Element* node);
};

#endif /* PROMNET_H_ */
