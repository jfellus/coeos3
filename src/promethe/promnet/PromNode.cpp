/*
 * PromNode.cpp
 *
 *  Created on: 6 nov. 2014
 *      Author: jfellus
 */

#include "PromNode.h"
#include <libxml++/libxml++.h>
#include <util/utils.h>
#include "PromNet.h"

using namespace xmlpp;

PromNode::~PromNode() {
	if(net) net->remove(this);
}

void PromNode::read(Element* node) {
	Node::NodeList list = node->get_children();
	for(Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter) {
		Element* node = dynamic_cast<Element*>(*iter);
		if(!node) continue;
		if(node->get_name()=="path") {
			Node::NodeList list = node->get_children();
			for(Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter) {
				Element* node = dynamic_cast<Element*>(*iter);
				if(!node) continue;
				TextNode* t = node->get_child_text();
				properties.set_from_string(std::string("path_")+node->get_name(),t ? (std::string)t->get_content() : "");
			}
		} else {
			TextNode* t = node->get_child_text();
			this->set_property(node->get_name(),t ? t->get_content() : "");
		}
	}
}

void PromNode::write(Element* node) {
	Element* path = NULL;
	for(uint i=0; i<properties.size(); i++) {
		Property* p = properties[i];
		if(str_starts_with(p->name, "path_")) {
			if(!path) {
				node->add_child_text("\n\t\t");
				path = node->add_child("path");
			}
			path->add_child_text("\n\t\t\t");
			Element* e = path->add_child(p->name.substr(strlen("path_")));
			e->add_child_text(p->get_value_as_string());
		} else {
			node->add_child_text("\n\t\t");
			Element* e = node->add_child(p->name);
			e->add_child_text(p->get_value_as_string());
		}
	}
	if(path) path->add_child_text("\n\t\t");
	node->add_child_text("\n\t");
}

void PromNode::init(PromScript* script) {
	script->node = this;
	this->script = script;
	script->node = this;
	properties.set("name", &script->name);
	properties.set("computer", "127.0.0.1");
	properties.set("login", "");
	properties.set("cmd", "promethe");
	properties.set("deploy", &script->path_deploy);
	properties.set("distant_directory", "");
	properties.set("keyboard", "");
	properties.set("synchronize_files", "");
	properties.set("options", "-S1 -D0 -W10");
	properties.set("path_script", &script->path_script);
	properties.set("path_symb", &script->path_symb);
	properties.set("path_draw", &script->path_draw);
	properties.set("path_res", &script->path_res);
	properties.set("path_config", &script->path_config);
	properties.set("path_bus", "");
	properties.set("path_dev", &script->path_dev);
	properties.set("path_gcd", &script->path_gcd);
	properties.set("path_prt", "");
}

void PromNode::realize() {
	try {
		script->load(script_filename);
	} catch(...) {ERROR("Can't load script " << script_filename); }
}
