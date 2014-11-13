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
	this->script = script;
	script->node = this;
	properties.set("name", script->name);
	properties.set_from_string("computer", "127.0.0.1");
	properties.set_from_string("login", "");
	properties.set_from_string("cmd", "promethe");
	properties.set("deploy", script->name);
	properties.set_from_string("distant_directory", "");
	properties.set_from_string("keyboard", "");
	properties.set_from_string("synchronize_files", "");
	properties.set_from_string("options", "-S1 -D0 -W10");
	properties.set_from_string("path_script", file_change_ext(script->name, ".script"));
	properties.set_from_string("path_symb", file_change_ext(script->name, ".symb"));
	properties.set_from_string("path_draw", file_change_ext(script->name, ".draw"));
	properties.set_from_string("path_res", file_change_ext(script->name, ".res"));
	properties.set_from_string("path_config", file_change_ext(script->name, ".config"));
	properties.set_from_string("path_bus", "");
	properties.set_from_string("path_dev", file_change_ext(script->name, ".dev"));
	properties.set_from_string("path_gcd", file_change_ext(script->name, ".gcd"));
	properties.set_from_string("path_prt", "");
}

void PromNode::realize() {
	try {
		script_filename =
				file_dirname(net->filename) + "/" + properties["deploy"]->get_value_as_string() + "/" +
				(properties["path_symb"]->get_value_as_string().empty() ? properties["path_script"]->get_value_as_string() :
						properties["path_symb"]->get_value_as_string());
		script = new PromScript();
		script->node = this;
		script->load(script_filename);

		script->name = properties["name"]->get_value_as_string();
	} catch(...) {script = NULL; }
}
