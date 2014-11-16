/*
 * PromNet.cpp
 *
 *  Created on: 6 nov. 2014
 *      Author: jfellus
 */

#include "PromNet.h"
#include <glibmm.h>
#include <libxml++/libxml++.h>

using namespace xmlpp;

void PromNet::realize() {
	for(uint i=0; i<nodes.size(); i++) {
		nodes[i]->realize();
	}
}

void PromNet::read(const std::string& filename) {
	try {
		this->filename = filename;
		DomParser parser;
		parser.parse_file(filename);
		if(parser) {
			const Node* pNode = parser.get_document()->get_root_node();
			if(pNode) {
				Node::NodeList list = pNode->get_children();
			    for(Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter) {
			      Element* node = dynamic_cast<Element*>(*iter);
			      if(!node) continue;
			      if(node->get_name()=="network") read_network(node);
			      else if(node->get_name()=="link_list") read_link_list(node);
			    }
			}
		}
	}
	catch(const std::exception& ex)  {  DBG(ex.what());   throw ex;  }
}

void PromNet::write(const std::string& filename) {
	Document* doc = new Document();
	Element* e = doc->create_root_node("application");
	e->add_child_text("\n\t");
	write_network(e);
	e->add_child_text("\n\t");
	write_link_list(e);
	e->add_child_text("\n");
	doc->write_to_file(filename);
}

void PromNet::read_network(Element* node) {
	 Node::NodeList list = node->get_children();
	 for(Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter) {
		  Element* node = dynamic_cast<Element*>(*iter);
		  if(!node) continue;
		  if(node->get_name()=="ivy_bus") { /* TODO */ }
		  else if(node->get_name()=="node") add(new PromNode(this, node));
	 }

	 for(uint i=nodes.size()-1; i>=0; i--) path_config = file_basename(nodes[i]->script->path_config);
}

void PromNet::read_link_list(Element* node) {
	// TODO
}

void PromNet::write_network(Element* node) {
	Element* net = node->add_child("network");
	net->add_child("ivy_bus");
	for(uint i=0; i<nodes.size(); i++) {
		net->add_child_text("\n\t");
		Element* e = net->add_child("node");
		nodes[i]->write(e);
	}
}

void PromNet::write_link_list(Element* node) {
	node->add_child("link_list");
}

