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

using namespace libboiboites;
namespace coeos {

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
				properties.set(std::string("path_")+node->get_name(),t ? (std::string)t->get_content() : "");
			}
		} else {
			TextNode* t = node->get_child_text();
			this->set_property(node->get_name(),t ? t->get_content() : "");
		}
	}

	properties.set("embedded",  file_is_absolute(get_filename()) ? "no" : "yes");
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

	if(file_has_ext(script->filename, ".symb")) path_symb = script->filename;
	if(file_has_ext(script->filename, ".script")) path_script = script->filename;

	properties.set("name", &script->name);
	properties.set("computer", "127.0.0.1");
	properties.set("login", "");
	properties.set("cmd", "promethe");
	properties.set("deploy", &path_deploy);
	properties.set("distant_directory", "");
	properties.set("keyboard", "");
	properties.set("synchronize_files", "");
	properties.set("options", "-S1 -D0 -W10");
	properties.set("path_script", &path_script);
	properties.set("path_symb", &path_symb);
	properties.set("path_draw", &path_draw);
	properties.set("path_res", &path_res);
	properties.set("path_config", &path_config);
	properties.set("path_bus", "");
	properties.set("path_dev", &path_dev);
	properties.set("path_gcd", &path_gcd);
	properties.set("path_prt", &path_prt);
	properties.set("path_var", &path_var);

	properties.set("embedded",  file_is_absolute(get_filename()) ? "no" : "yes");

	add_properties_listener(this);
}

void PromNode::realize() {
	try {
		if(path_symb.empty() && path_script.empty()) { ERROR("Script " << script->name << " doesn't define a .symb or .script file"); return; }
		script->load(get_absolute_filename());

		properties.set("embedded",  file_is_absolute(get_filename()) ? "no" : "yes");
	} catch(...) {ERROR("Can't load script " << get_absolute_filename()); }
}


void PromNode::on_property_change(IPropertiesElement* m, const std::string& name, const std::string& val) {
	if(val.empty()) return;
	if(name=="path_script") properties.set("path_script", file_change_ext(properties.get_as_string("path_script"), ".script"));
	if(name=="path_symb") properties.set("path_symb", file_change_ext(properties.get_as_string("path_symb"), ".symb"));
	if(name=="path_draw") properties.set("path_draw", file_change_ext(properties.get_as_string("path_draw"), ".draw"));
	if(name=="path_res") properties.set("path_res", file_change_ext(properties.get_as_string("path_res"), ".res"));
	if(name=="path_config") properties.set("path_config", file_change_ext(properties.get_as_string("path_config"), ".config"));
	if(name=="path_dev") properties.set("path_dev", file_change_ext(properties.get_as_string("path_dev"), ".dev"));
	if(name=="path_gcd") properties.set("path_gcd", file_change_ext(properties.get_as_string("path_gcd"), ".gcd"));
	if(name=="path_prt") properties.set("path_prt", file_change_ext(properties.get_as_string("path_prt"), ".prt"));
	if(name=="path_var") properties.set("path_var", file_change_ext(properties.get_as_string("path_var"), ".var"));

	if(name=="path_symb") properties.set("embedded",  file_is_absolute(path_symb) ? "no" : "yes");
	if(name=="path_script") properties.set("embedded",  file_is_absolute(path_script) ? "no" : "yes");
	if(name=="embedded") {
		if(val=="yes") {
			if(!path_symb.empty() && file_is_absolute(path_symb)) path_symb = file_basename(path_symb);
			if(!path_script.empty() && file_is_absolute(path_script)) path_script = file_basename(path_script);
		}
	}
}


std::string PromNode::get_absolute_filename() {
	std::string fn = get_filename();
	if(!net || file_is_absolute(fn)) return fn;
	return net->get_dir() + "/" + fn;
}

std::string PromNode::get_filename() {
	std::string fn = path_symb;
	if(fn.empty()) fn = path_script;
	if(fn.empty()) fn = path_symb = file_change_ext(script->name, ".symb");
	if(!net || file_is_absolute(fn)) return fn;
	if(path_deploy.empty()) return fn;
	return path_deploy + "/" + fn;
}

std::string PromNode::get_absolute_path(const std::string& filename) {
	if(filename.empty()) return "";
	if(!net || file_is_absolute(filename)) return filename;
	return net->get_dir() + "/" + path_deploy + "/" + filename;
}

void PromNode::set_script_name(const std::string& name) {
	script->name = name;

}


}
